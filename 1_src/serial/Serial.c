/**!
 * Copyright (C), xxx
 * @file    Serial.c
 * @brief   Linux应用层串口驱动
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <termios.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include "Serial.h"
#include "merrno.h"
#include "defines.h"

/*******************************************************************************
 * @brief       设置串口波特率，数据位，奇偶校验，停止位
 * @param[in]   serial_name    串口名称
 * @param[in]   BaudSpeed      波特率大小
 * @param[in]   DataBits       数据位数
 * @param[in]   Parity         奇偶校验
 * @param[in]   StopBits       停止位数
 * @param[out]  无
 * @return      0，执行成功，非0，失败
 ******************************************************************************/
int Serial_open(char *serial_name, int BaudSpeed, int DataBits, char Parity,
                int StopBits)
{
	struct termios tty;
    int fd;

	memset(&tty, 0, sizeof tty);
	fd = open(serial_name, O_RDWR | O_NOCTTY | O_NDELAY);
	if (tcgetattr(fd, &tty) != 0) {
		pr_error ("Error %d from tggetattr!\n", errno);
		return -MENXIO;
    }

	tty.c_cflag |= (CLOCAL | CREAD);
	switch (DataBits) {                     /* Set databits */
	case 7:
		tty.c_cflag &= ~CSIZE;
		tty.c_cflag |= CS7;
		break;
	case 8:
		tty.c_cflag &= ~CSIZE;
		tty.c_cflag |= CS8;
		break;
	default:
		pr_error("Unsupported data size!\n");
		return -MEINVAL;
    }

	switch (Parity) {                       /* Set Parity */
	case 'n':
	case 'N':
		tty.c_cflag &= ~PARENB;             /* Clear parity enable */
		tty.c_iflag &= ~INPCK;              /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		tty.c_cflag |= (PARODD | PARENB);   /* 设置为奇效验 */
		tty.c_iflag |= INPCK;               /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		tty.c_cflag |= PARENB;
		tty.c_cflag &= ~PARODD;             /* 设置为偶效验b*/
		tty.c_iflag |= INPCK;               /* Disnable parity checking */
		break;
	default:
		pr_error("Unsupported parity!\n");
		return -MEINVAL;
	}

	switch (BaudSpeed) {                    /* Set BaudSpeed */
	case 300:
		cfsetospeed(&tty, B300);
		cfsetispeed(&tty, B300);
		break;
	case 1200:
		cfsetispeed(&tty, B1200);
		cfsetospeed(&tty, B1200);
		break;
	case 2400:
		cfsetospeed(&tty, B2400);
		cfsetispeed(&tty, B2400);
		break;
	case 4800:
		cfsetispeed(&tty, B4800);
		cfsetospeed(&tty, B4800);
		break;
	case 9600:
		cfsetispeed(&tty, B9600);
		cfsetospeed(&tty, B9600);
		break;
	case 19200:
		cfsetispeed(&tty, B19200);
		cfsetospeed(&tty, B19200);
		break;
	case 38400:
		cfsetispeed(&tty, B38400);
		cfsetospeed(&tty, B38400);
		break;
	case 115200:
		cfsetispeed(&tty, B115200);
		cfsetospeed(&tty, B115200);
		break;
	default:
		cfsetispeed(&tty, B9600);
		cfsetospeed(&tty, B9600);
		break;
	}

	switch (StopBits) {
	case 1:
		tty.c_cflag &= ~CSTOPB;
		break;
	case 2:
		tty.c_cflag |= CSTOPB;
		break;
	default:
		pr_error("Unsupported stop bit!\n");
		return -MEINVAL;
	}
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	//tty.c_lflag &= ~(ICANON | ISIG);
	tty.c_oflag &= ~OPOST;
	tty.c_oflag &= ~(ONLCR | OCRNL);
	tty.c_iflag &= ~(ICRNL | INLCR);
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);

	tty.c_cc[VMIN]  = 0;                        /* read doesn't block */
	tty.c_cc[VTIME] = 0;                        /* 0 seconds read timeout */
	tcflush(fd, TCIFLUSH);
	if (tcsetattr (fd, TCSANOW, &tty) != 0) {   /* 设置终端参数 */
		pr_error("SetupSerial error!\n");
		return -MEBFONT;
	}

    pr_detail("Serial opened\n");

	return fd;
}

/*******************************************************************************
 * @brief       读串口数据
 * @param[in]   fd        串口文件句柄
 * @param[in]   count     要读的最大数据字节数
 * @param[in]   timeout   延迟时间(ms)
 * @param[out]  buf       存放所读到的数据缓冲区
 * @return      0，执行成功，非0，失败
 ******************************************************************************/
ssize_t Serial_read(int fd, char *buf, size_t count, unsigned int timeout)
{
	struct termios tty;
	ssize_t Num_bytes = 0;

	memset(&tty, 0, sizeof tty);
	if (tcgetattr(fd, &tty) != 0) {
		pr_error("Error %d from tggetattr!\n", errno);
		return -MENXIO;
	}

	timeout /= 100;
	tty.c_cc[VTIME] = timeout;                  /* 0 seconds read timeout */

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {    /* 设置终端参数 */
		pr_error("SetupSerial error!\n");
		return -MEBFONT;
	}

    /* Please using select() if block */
	Num_bytes = read(fd, buf, count);

	return Num_bytes;
}

/*******************************************************************************
 * @brief       向串口写数据
 * @param[in]   fd      串口文件句柄
 * @param[in]   count   要发送的最大数据字节数
 * @param[in]   buff    存放要发送的数据缓冲区
 * @param[out]  无
 * @return      0，执行成功，非0，失败
 ******************************************************************************/
ssize_t Serial_write(int fd,const char *buf,size_t count)
{
	ssize_t info;

	info = write(fd, buf, count);

	return info;
}

/*******************************************************************************
 * @brief       关闭串口
 * @param[in]   fd    串口文件句柄
 * @param[out]  无
 * @return        0，执行成功，非0，失败
 ******************************************************************************/
int Serial_close(int fd)
{
	int info;

	info = close(fd);

	return info;
}
