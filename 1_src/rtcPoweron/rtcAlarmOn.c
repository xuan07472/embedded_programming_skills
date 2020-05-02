/**
 * RTC Alarm of Smart Ocean Tech Angelfish Omapl138 board.
 */

#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

//https://blog.csdn.net/zhangxiaopeng0829/article/details/7514061
//直接通过sysfs操作的话可以操作/sys/class/rtc/rtc0
//要测定时开机的话操作echo "+NNN" > /sys/class/rtc/rtc0/wakealarm(我们板子没有此文件)
//echo "enabled" > /sys/class/rtc/rtc0/power/wakeup(我们板子没有此文件)
//cat /proc/driver/rtc   rtc  查看更详细的信息

/*
 * This expects the new RTC class driver framework, working with
 * clocks that will often not be clones of what the PC-AT had.
 * Use the command line to specify another RTC if you need one.
 */
static const char default_rtc[] = "/dev/rtc0";
#define LEAPYEAR(Y) (!(Y % 400) || !(Y % 4) && (Y % 100))
int day_of_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/* @brief   Set rtc alarm.
 * @param(argc) the number of argument(do not need input), 1: used default, 2: used argument
 * @param(argv)
 */
int main(int argc, char *argv[]) {
	int delay_min, total_day;
	int i, fd, retval, irqcount = 0;
	unsigned long tmp, data;
	struct rtc_time rtc_tm;
	const char *rtc = default_rtc;
	struct timeval start, end, diff;

	if (argc != 2) {
		printf("Usage: %s alarm_delay(mins)\n", argv[0]);
		return -1;
	}

	delay_min = atoi(argv[1]);

	fd = open(rtc, O_RDONLY);
	if (-1 == fd) {
		perror(rtc);
		exit(errno);
	}

	fprintf(stdout, "Set RTC Alarm.\n");

	/* Disable alarm interrupts */
	retval = ioctl(fd, RTC_AIE_OFF, 0);
	if (retval == -1) {
		perror("\nRTC_AIE_OFF ioctl, Disable alarm err!");
		exit(errno);
	}
	fprintf(stdout, "Disable alarm ok!\n");

test_READ:
	/* Read the RTC time/date */
	retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (-1 == retval) {
		perror("\nRTC_RD_TIME ioctl, read rtc err!");
		exit(errno);
	}

	fprintf(stdout, "Current RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
		rtc_tm.tm_year + 1900, rtc_tm.tm_mon + 1, rtc_tm.tm_mday,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

test_ALARM:
	/* Set the alarm to X sec in the future, and check for rollover */
	fprintf(stdout, "...Start to test alarm:\n");

    /* Premise: not in the critical point now. */
	rtc_tm.tm_sec = 0;
	rtc_tm.tm_min += delay_min - rtc_tm.tm_min % delay_min;
	while (rtc_tm.tm_min >= 60) {
		rtc_tm.tm_min -= 60;
		rtc_tm.tm_hour++;
	}
	while (rtc_tm.tm_hour >= 24) {
		rtc_tm.tm_hour -= 24;
		rtc_tm.tm_mday++;
	}

	total_day = day_of_month[rtc_tm.tm_mon];
	total_day += rtc_tm.tm_mon == (2 - 1) ? LEAPYEAR(rtc_tm.tm_year) : 0;
	if (rtc_tm.tm_mday > total_day) {
		rtc_tm.tm_mday -= total_day;
		rtc_tm.tm_mon += 1;
	}
	if (rtc_tm.tm_mon > (12 - 1)) {
		rtc_tm.tm_mon -= (12 - 1);
		rtc_tm.tm_year++;
	}

	fprintf(stdout, "Alarm time want set to %04d.%02d.%02d-%02d:%02d:%02d.\n",
        rtc_tm.tm_year + 1900, rtc_tm.tm_mon + 1, rtc_tm.tm_mday,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
	retval = ioctl(fd, RTC_ALM_SET, &rtc_tm);
	if (-1 == retval) {
		if (EINVAL == errno) {
			fprintf(stderr,
				"\n...Alarm IRQs not supported, ALM_SET err!\n");
			goto done;
		}

		perror("\nRTC_ALM_SET ioctl");
		exit(errno);
	}
	fprintf(stdout, "RTC_ALM_SET ok!\n");

	/* Read the current alarm settings */
	retval = ioctl(fd, RTC_ALM_READ, &rtc_tm);
	if (retval == -1) {
		perror("\nRTC_ALM_READ ioctl");
		exit(errno);
	}

	fprintf(stdout, "RTC_ALM_READ ok!\n");
	fprintf(stdout, "Alarm time now set to %04d.%02d.%02d-%02d:%02d:%02d.\n",
        rtc_tm.tm_year + 1900, rtc_tm.tm_mon + 1, rtc_tm.tm_mday,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	/* Enable alarm interrupts */
	retval = ioctl(fd, RTC_AIE_ON, 0);
	if (-1 == retval) {
		if (EINVAL == errno) {
			fprintf(stderr,
				"\n...Alarm IRQs not supported, AIE_ON err\n");
			goto done;
		}

		perror("\nRTC_AIE_ON ioctl!");
		exit(errno);
	}

	fprintf(stdout, "Enable alarm ok!\n");

	fflush(stderr);

done:
	fprintf(stdout, "\t\t\t *** Rtc alarm set complete ***\n");

	close(fd);

	return 0;
}
