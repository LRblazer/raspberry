/*********************************************************************************
 *      Copyright:  (C) big world 
 *                  All rights reserved.
 *
 *       Filename:  comport.h
 *    Description:  This head file is linux serial port operate API
 *                   
 *        Version:  1.0.0(7/17/2020)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "7/17/2020 03:33:25 PM"
 *                     
 ********************************************************************************/
#ifndef  __COMPORT_H_
#define  __COMPORT_H_

#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <string.h>
#include  <getopt.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <termios.h>
#include  <sys/stat.h>
#include  <sys/wait.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/select.h>

#define BUF_64  64

#ifndef DEVNAME_LEN
#define DEVNAME_LEN          64
#endif

//#define COM_DEBUG
#ifdef  COM_DEBUG
#define COM_PRINT(format,args...) printf(format, ##args)
#else
#define COM_PRINT(format,args...) do{} while(0);
#endif

//#define msleep(m)               {struct timespec cSleep; cSleep.tv_sec = 0; cSleep.tv_nsec = m * 1000; nanosleep(&cSleep, 0);}

typedef struct comport_s
{
    char           dev_name[DEVNAME_LEN];
    unsigned char  databit, parity, stopbit, flowctrl;
    long           baudrate;

    int            fd;
    int            frag_size;
} comport_t;

/*
 *  description: initialise the comport structure
 *
 *   input args: $dev_name:  The comport device name path, such as '/dev/ttyS3'
 *               $baudrate:  The baudrate, such as 115200
 *               $settings:  The databit,parity,stopbit,flowctrl settings, such as '8N1N'
 *
 * return value: The comport_t structure pointer, NULL means failure.
 */
  
comport_t *comport_init(const char *dev_name, int baudrate, const char *settings);


/* 
 *  description: Open the comport specified by $comport
 *   input args: $comport:  corresponding comport point 
 * return value: The comport opened file description, <0 means failure
 */
extern int  comport_open(comport_t * comport);

/*
 *  description: read data from $comport in $timeout <ms> to $buf no more than $buf_size bytes
 * return value: the actual read data bytes, <0: read failure
 */
extern int  comport_recv(comport_t * comport, char *buf, int buf_size, unsigned long timeout);

/*
 *  description: write $send_bytes bytes data from $buf to $comport
 * return value: 0: write ok  <0: write failure
 */
extern int  comport_send(comport_t * comport, char *buf, int send_bytes);

/*
 *  description: display current comport settings such as databit,parity,stopbit,flowctrl
 *   input args: $comport:  corresponding comport point 
 */
//extern void disp_settings(comport_t * comport);

/*
 *  description: close comport 
 *   input args: $comport:  corresponding comport point 
 */
extern void comport_close(comport_t * comport);


/*
 *  description: terminat comport, close and free it
 *   input args: $comport:  corresponding comport point 
 */
extern void comport_term(comport_t * comport);

#endif
