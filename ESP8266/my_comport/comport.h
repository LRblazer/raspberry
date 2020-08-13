/*********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  comport.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(30/07/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "30/07/20 03:11:38"
 *                 
 ********************************************************************************/

#ifndef _COMPORT_H_
#define _COMPORT_H_

#include<stdio.h>      /*标准输入输出定义*/
#include<stdlib.h>     /*标准函数库定义*/
#include<unistd.h>     /*Unix 标准函数定义*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>      /*文件控制定义*/
#include<termios.h>    /*PPSIX 终端控制定义*/
#include<errno.h>      /*错误号定义*/
#include<string.h>
#include <signal.h>
#include <getopt.h>


#define FALSE  -1
#define TRUE   0

/* 自定义的串口结构体 */
typedef struct com_attr
{
   unsigned  int  flow_ctrl; /* 流控 */
  unsigned   int  speed;     /*波特率*/
  unsigned   int databits;     /*数据位*/
  unsigned   char parity;       /*奇偶校验位*/
  unsigned   int stopbits;     /* 停止位 */
}UART_attr;




/***************************************
 * name:        UART_open()
 * function:    open UART 
 * inlet parameter: fd, port  
 * outlet parameter: 
 * *************************************/
int UART_open(int fd,char* port);


/*******************************************************************
 * 名称：                UART_close
 * 功能：                关闭串口并返回串口设备文件描述
 * 入口参数：            fd:文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)
 * 出口参数：            void
 *******************************************************************/

void UART_close(int fd);

/*******************************************************************
 * 名称：                UART_set
 * 功能：                设置串口数据位，停止位和效验位
 * 入口参数：            fd          串口文件描述符
 *                       speed       串口速度
 *                       flow_ctrl   数据流控制
 *                       databits    数据位   取值为 7 或者8
 *                       stopbits    停止位   取值为 1 或者2
 *                       parity      奇偶校验位类型 取值为N,E,O,,S
 *出口参数：             正确返回为1，错误返回为0
 *******************************************************************/
int UART_set(int fd, UART_attr *attr);



/*******************************************************************
 * 名称：                UART_Init()
 * 功能：                串口初始化,默认设置
 * 入口参数：            fd         文件描述符
 *                       speed      串口速度
 *                       flow_ctrl  数据流控制
 *                       databits   数据位   取值为 7 或者8
 *                       stopbits   停止位   取值为 1 或者2
 *                       parity     效验类型 取值为N,E,O,,S
 *
 * 出口参数：            正确返回为1，错误返回为0
 *******************************************************************/
int UART_Init(int fd);

/*******************************************************************
 * 名称：                  UART_Recv
 * 功能：                  接收串口数据
 * 入口参数：              fd          :文件描述符
 *                         rcv_buf     :接收串口中数据存入rcv_buf缓冲区中
 *                         data_len    :一帧数据的长度
 * 出口参数：              正确返回为1，错误返回为0
 *******************************************************************/
int UART_recv(int fd,char *rbuf,int rbuf_len,int timeout);


/********************************************************************
 * 名称：                  UART_Send
 * 功能：                  发送数据
 * 入口参数：        fd                  :文件描述符
 *                              send_buf    :存放串口发送数据
 *                              data_len    :一帧数据的个数
 * 出口参数：        正确返回为1，错误返回为0
 *******************************************************************/
#define max_send_len 128

int UART_send(int fd,char *sbuf,int sbuf_len);


#endif
