/**!
 * Copyright (C), xxx
 * @file    Serial.h
 * @brief   Linux应用层串口驱动
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>

int Serial_open(char *serial_name,int BaudSpeed,int DataBits,char Parity,int StopBits);
int Serial_close(int fd);
ssize_t Serial_read(int fd,char *buf,size_t count, unsigned int timeout);
//#define Serial_read_b(fd, buf, count)	Serial_read(fd, buf, count, (unsigned int)-1)
ssize_t Serial_read(int fd, char *buf, size_t count, unsigned int timeout);
ssize_t Serial_write(int fd,const char *buf,size_t count);

#endif /* _SERIAL_H_ */
