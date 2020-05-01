/**
 * GPIO option application
 */

#if 0 /* Operate the gpio driver. */
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
#endif

/*
    在操作系统中直接使用命令行操作。
    GPIO可以不使用驱动模块，直接调用/sys/class/gpio设置gpio复用引脚，
操作，操作值，通过查询的方式获取中断等。
    操作的GPIO值=block*16+pin，GPIO8_15 = pin 8*16+15 = pin143
    echo 143 > /sys/class/gpio/export  设置并导出引脚
    echo in > /sys/class/gpio/gpio143/direction    设置方向
    cat /sys/class/gpio/gpio143/direction    查看方向
    echo 1 /sys/class/gpio/gpio143/value    设置输出值
    cat /sys/class/gpio/gpio143/value    查看输入值
    echo 143 >/sys/class/gpio/unexport    取消导出
    echo rising > /sys/class/gpio/gpio143/edge    设置为上升沿触发，否则为none不触发
*/

/**
 * 通过查询/sys/class/gpio的文件来获取GPIO中断
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/select.h>
#include <sys/time.h>

int main(void)
{
    /* 设置SD1_D0为GPIO输入引脚，中断为上升沿触发 */
    printf(">>Unexport GPIO8_15.\n");
    system("echo 143 > /sys/class/gpio/unexport");

    printf(">>Export GPIO8_15.\n");
    system("echo 143 > /sys/class/gpio/export");

    printf(">>Set input direction of GPIO8_15.\n");
    system("echo in > /sys/class/gpio/gpio143/direction");

    printf(">>Cat the direction of GPIO8_15:\n");
    system("cat /sys/class/gpio/gpio143/direction");

    printf(">>Cat the input value of GPIO8_15:\n");
    system("cat /sys/class/gpio/gpio143/value");
    
    printf(">>Set the interrupt type of GPIO8_15:\n");
    system("echo rising > /sys/class/gpio/gpio143/edge");

/*    // Use epoll.
    struct epoll_event evd;
    struct epoll_event * events;
    int epollfd = epoll_create(10);
    int fd;
    char buf[16];
    int n, i;
       events = calloc (10, sizeof(struct epoll_event));
    fd = open("/sys/class/gpio/gpio143/value", O_RDONLY);
    if (fd < 0) {
        printf(">File open fail!\n");
        return 0;
    }exceptfds
    evd.data.fd = fd;
    evd.events = EPOLLPRI;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&evd); 
    while (1) {
        n = epoll_wait(epollfd,events,10,-1);
        for (i = 0;i < n;i++) {
            if (events[i].events & EPOLLPRI) {
                memset(buf,0x00,sizeof(buf));
                read(events[i].data.fd,buf,sizeof(buf));
                lseek(events[i].data.fd,0,SEEK_SET);
                printf(">Get interrupt:%c.\n", buf[0]);
            }
        }
    }
*/

/*    // Use poll.
    struct pollfd fds;
    struct pollfd *xfds = &fds;
    int fd;
    int ret;
    char buf[2];

    fd = open("/sys/class/gpio/gpio143/value", O_RDONLY);
    memset((void *)xfds, 0, sizeof(xfds));
    xfds[0].fd = fd;
    xfds[0].events = POLLPRI;
    
    while (1) {
        ret = poll(xfds, 1, -1);
        if(ret <= 0) {
            printf(">Poll value err!\n");
            return 0;
        }
        if(xfds[0].revents & POLLPRI)
        {
            ret = lseek(fd, 0, SEEK_SET);
            if(ret < 0) {
                printf(">Lseek value err!\n");
                return 0;
            }
            ret = read(fd, buf, 2);
            buf[1] = '\0';
                printf(">Read ret = %d, value = %s\n", ret, buf);
            if(ret != 2) {
                printf(">Read value err!\n");
                return 0;
            }
        }
    }
*/

    /* 在应用层获取GPIO中断，并打印当次中断与前一次中断的间隔时间 */
    // Use select
    int fd;
    fd_set exceptfds;
    char buf[2];
    int ret;
    struct timeval tv;
    struct timeval tv_bak = {0, 0};
    struct timezone tz;
    long tmp_sec = 0;
    long tmp_msec = 0;

    fd = open("/sys/class/gpio/gpio143/value", O_RDONLY);
    if (fd < 0)
        printf(">>Open error!\n");
    FD_ZERO(&exceptfds);
    FD_SET(fd, &exceptfds);

    while (1) {
        ret = select(fd+1,NULL,NULL,&exceptfds,NULL);
        if(ret < 0) {
            printf(">>Select value err!\n");
            return 0;
        }
        if(ret > 0)
        {
            ret = lseek(fd, 0, SEEK_SET);
            if(ret < 0) {
                printf(">>Lseek value err!\n");
                return 0;
            }
            ret = read(fd, buf, 2);
            buf[1] = '\0';
//            printf(">Read ret = %d, value = %c\n", ret, buf[0]);
            if(ret != 2) {
                printf(">>Read value err!\n");
                return 0;
            }
            gettimeofday(&tv, &tz);
            tmp_msec = (tv.tv_usec - tv_bak.tv_usec) / 1000;
            if (tmp_msec >= 0)
                tmp_sec = tv.tv_sec - tv_bak.tv_sec;
            else {
                tmp_sec = tv.tv_sec - tv_bak.tv_sec - 1;
                tmp_msec = 1000 - tmp_msec;
            }
            if (tmp_sec > 0)
                printf("\n\n");
            printf(">Delta time: [[%01d.%03ds]], intr val:%c.\n",
                            tmp_sec, tmp_msec, buf[0]);
            memcpy(&tv_bak, &tv, sizeof(struct timeval));
        }
    }

    /* BUG1：加载DSP程序后则无法触发中断（可能是DSP程序初始化了SD1，要删掉DSP该部分代码） */
    /* BUG2：发送波形时前导码信息也能多次触发中断，此中断和正常触发的中断的间隔时间有时又小于150ms？？  */
    return 0;
}
