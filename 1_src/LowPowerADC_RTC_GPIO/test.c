#if 0
/* ADC device */
#include <stdio.h>

int main(void)
{
    FILE *fd;
    size_t ret;
    char a_data[16] = {0};
    
    fd = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_scale", "r+");
    if (fd == NULL) {
        printf("->Open in_voltage1_scale fail!\n");
        return 0;
    }
    
    ret = fwrite("01", 3, 1, fd);
    if (ret != 1) {
        printf("->Write data fail!\n");
    }
    fread(a_data, 15, 1, fd);
    printf("->The data:%s.\n", a_data);
    fclose(fd);
    return 0;
}
#endif

/*
 *      Real Time Clock Driver Test/Example Program
 *
 *      Compile with:
 *		     gcc -s -Wall -Wstrict-prototypes rtctest.c -o rtctest
 *
 *      Copyright (C) 1996, Paul Gortmaker.
 *
 *      Released under the GNU General Public License, version 2,
 *      included herein by reference.
 *
 */

#if 0
#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


/*
 * This expects the new RTC class driver framework, working with
 * clocks that will often not be clones of what the PC-AT had.
 * Use the command line to specify another RTC if you need one.
 */
static const char default_rtc[] = "/dev/rtc0";


int main(int argc, char **argv)
{
	int i, fd, retval, irqcount = 0;
	unsigned long tmp, data;
	struct rtc_time rtc_tm;
	const char *rtc = default_rtc;
	struct timeval start, end, diff;

	switch (argc) {
	case 2:
		rtc = argv[1];
		/* FALLTHROUGH */
	case 1:
		break;
	default:
		fprintf(stderr, "usage:  rtctest [rtcdev]\n");
		return 1;
	}

	fd = open(rtc, O_RDONLY);

	if (fd ==  -1) {
		perror(rtc);
		exit(errno);
	}

	fprintf(stderr, "\n\t\t\tRTC Driver Test Example.\n\n");

test_READ:
	/* Read the RTC time/date */
	retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (retval == -1) {
		perror("RTC_RD_TIME ioctl");
		exit(errno);
	}

	fprintf(stderr, "\n\nCurrent RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
		rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	fprintf(stderr,
		"\n\n...Start to test alarm:\n");
	/* Set the alarm to 5 sec in the future, and check for rollover */
	rtc_tm.tm_sec += 5;
	if (rtc_tm.tm_sec >= 60) {
		rtc_tm.tm_sec %= 60;
		rtc_tm.tm_min++;
	}
	if (rtc_tm.tm_min == 60) {
		rtc_tm.tm_min = 0;
		rtc_tm.tm_hour++;
	}
	if (rtc_tm.tm_hour == 24)
		rtc_tm.tm_hour = 0;

	retval = ioctl(fd, RTC_ALM_SET, &rtc_tm);
	if (retval == -1) {
		if (errno == EINVAL) {
			fprintf(stderr,
				"\n...Alarm IRQs not supported, ALM_SET err.\n");
			goto done;
		}

		perror("RTC_ALM_SET ioctl");
		exit(errno);
	}
	fprintf(stderr,
		"RTC_ALM_SET ok!\n");
	/* Read the current alarm settings */
	retval = ioctl(fd, RTC_ALM_READ, &rtc_tm);
	if (retval == -1) {
		perror("RTC_ALM_READ ioctl");
		exit(errno);
	}

	fprintf(stderr,
		"RTC_ALM_READ ok!\n");
	fprintf(stderr, "Alarm time now set to %02d:%02d:%02d.\n",
		rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
	
	/* Enable alarm interrupts */
	retval = ioctl(fd, RTC_AIE_ON, 0);
	if (retval == -1) {
		if (errno == EINVAL) {
			fprintf(stderr,
				"\n...Alarm IRQs not supported, AIE_ON err\n");
			goto done;
		}

		perror("RTC_AIE_ON ioctl");
		exit(errno);
	}
	
	fprintf(stderr,
		"RTC_AIE_ON ok!\n");

	fprintf(stderr, "Waiting 5 seconds for alarm...");
	fflush(stderr);
	sleep(5);

	fprintf(stderr, " okay. Alarm rang.\n");

	/* Disable alarm interrupts */
	/*
	retval = ioctl(fd, RTC_AIE_OFF, 0);
	if (retval == -1) {
		perror("RTC_AIE_OFF ioctl");
		exit(errno);
	}
	fprintf(stderr,
		"RTC_AIE_OFF ok!\n");
		*/

done:
	fprintf(stderr, "\n\n\t\t\t *** Test complete ***\n");

	close(fd);

	return 0;
}
#endif

#if 0
#include <stdio.h>
/**
 * Test the power.
 */
int main(void)
{
  int i, j = 0;
  for (i=0; i<1000000000; i++) {
    j *= 2;
  }
  printf("i:%d\n");
}
#endif

/* GPIO option */
/*
    GPIO可以不使用驱动模块，直接调用/sys/class/gpio设置gpio复用引脚，
操作，操作值，通过查询的方式获取中断等。
    操作的GPIO值=block*16+pin，GPIO8_15 = pin 8*16+15 = pin143
    /sys/class/gpio# echo 143 > export  设置并导出引脚
    echo out > gpio143/direction    设置方向
    cat /sys/class/gpio/gpio143/direction    查看方向
    echo 1 > /sys/class/gpio/gpio143/value    设置输出
    cat /sys/class/gpio/gpio143/value    查看输出
    echo 143 > /sys/class/gpio/unexport    取消导出
*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define GPIO_TEST_IOC_MAGIC          0x92
#define SET_OUTPUT_LOW _IO(GPIO_TEST_IOC_MAGIC, 0)
#define SET_OUTPUT_HIGH _IO(GPIO_TEST_IOC_MAGIC, 1)
#define GET_VALUE _IO(GPIO_TEST_IOC_MAGIC, 2)
#define SET_INPUT _IO(GPIO_TEST_IOC_MAGIC, 3)

int main(void)
{
    int fd, value;
    
    fd = open("/dev/omapl138_gpios", O_RDWR);
    if (fd < 0) {
        printf("Open err!\n");
        return -1;
    }
    value = ioctl(fd, GET_VALUE, 815); // 0设低 1设高 2读 GPIO8_15
    printf("The gpio value: %d.\n", value);
    
    close(fd);
    
    return 0;
}


