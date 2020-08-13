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
#include "comport.h"





/***************************************
 * name:        UART_open()
 * function:    open UART 
 * inlet parameter:
 * outlet parameter:
 * *************************************/
int UART_open(int fd,char* port)
{
    /* O_NOCTTY 如果路径名指向终端设备，不要把这个设备用作控制终端。
     *
     * O_NONBLOCK 阻塞模式 */
    fd = open( port, O_RDWR|O_NOCTTY|O_NONBLOCK );
    if (FALSE == fd)
    {
        perror("Can't Open Serial Port");
        /* printf("open serial port failed %s", strerror(errno)); */
        return(FALSE);
    }

    //判断串口是否为阻塞状态
    if(fcntl(fd, F_SETFL, 0) < 0)
    {
        printf("fcntl failed!\n");
        return(FALSE);
    }
    else
    {
        printf("fcntl=%d\n",fcntl(fd, F_SETFL,0));
    }

    //测试是否为终端设备
    if(0 == isatty(STDIN_FILENO))
    {
        printf("standard input is not a terminal device\n");
        return(FALSE);
    }
    else
    {
        printf("isatty success!\n");
    }
    printf("fd->open=%d\n",fd);
    return fd;
}


/*******************************************************************
 * 名称：                UART_close
 * 功能：                关闭串口并返回串口设备文件描述
 * 入口参数：            fd:文件描述符     port :串口号(ttyS0,ttyS1,ttyS2)
 * 出口参数：            void
 *******************************************************************/

void UART_close(int fd)
{

    tcflush(fd, TCIOFLUSH);
    close(fd);
}

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
int UART_set(int fd, UART_attr *attr)
{

    int   i;
    int   status;
    int   speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};
    int   name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};
    struct termios options;

    /*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1.
    */
    if ( tcgetattr( fd,&options)  !=  0)
    {
        perror("SetupSerial 1");
        return(FALSE);
    }

    //设置串口输入波特率和输出波特率
    for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
    {
        if(attr->speed == name_arr[i])
        {
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
        }
    }

    //修改控制模式，保证程序不会占用串口
    options.c_cflag |= CLOCAL;
    //修改控制模式，使得能够从串口中读取输入数据
    options.c_cflag |= CREAD;

    //设置数据流控制
    switch(attr->flow_ctrl)
    {

        case 0 ://不使用流控制
            options.c_cflag &= ~CRTSCTS;
            break;

        case 1 ://使用硬件流控制
            options.c_cflag |= CRTSCTS;
            break;
        case 2 ://使用软件流控制
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;
    }
    //屏蔽其他标志位
    options.c_cflag &= ~CSIZE;



    //设置数据位
    switch (attr->databits)
    {
        case 5:
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr,"Unsupported data size\n");
            return (FALSE);
    }

    //设置校验位
    switch (attr->parity)
    {
        case 'n':
        case 'N': //无奇偶校验位。
            options.c_cflag &= ~PARENB;
            options.c_iflag &= ~INPCK;
            break;
        case 'o':
        case 'O'://设置为奇校验
            options.c_cflag |= (PARODD | PARENB);
            options.c_iflag |= INPCK;
            break;
        case 'e':
        case 'E'://设置为偶校验
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_iflag |= INPCK;
            break;
        case 's':
        case 'S': //设置为空格
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            fprintf(stderr,"Unsupported parity\n");
            return (FALSE);
    }


    // 设置停止位
    switch (attr->stopbits)
    {
        case 1:
            options.c_cflag &= ~CSTOPB; break;
        case 2:
            options.c_cflag |= CSTOPB; break;
        default:
            fprintf(stderr,"Unsupported stop bits\n");
            return (FALSE);
    }


    //修改输出模式，原始数据输出
    options.c_oflag &= ~OPOST;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);//我加的
    //options.c_lflag &= ~(ISIG | ICANON);

    //设置等待时间和最小接收字符
    options.c_cc[VTIME] = 0; /* 读取一个字符等待1*(1/10)s */
    options.c_cc[VMIN] = 0; /* 读取字符的最少个数为1 */

    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读
    tcflush(fd,TCIFLUSH);

    //激活配置 (将修改后的termios数据设置到串口中）
    if (tcsetattr(fd,TCSANOW,&options) != 0)
    {
        perror("com set error!\n");
        return (FALSE);
    }
    printf("set down\n");
    return (TRUE);
}


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
int UART_Init(int fd)
{
  //  UART_attr *old_opt={0,115200,'8','N', '0'};

  //  UART_set(fd, old_opt);
/*    //设置串口数据帧格式
    if(tcsetattr(fd, TCSANOW, &old_opt))
    {
   //     printf("%s,init failed:%s", FUNCTION, strerror(errno));
        return FALSE;
    }
    else
    {
        return  TRUE;
    }*/
}

/*******************************************************************
 * 名称：                  UART_Recv
 * 功能：                  接收串口数据
 * 入口参数：              fd          :文件描述符
 *                         rcv_buf     :接收串口中数据存入rcv_buf缓冲区中
 *                         data_len    :一帧数据的长度
 * 出口参数：              正确返回为1，错误返回为0
 *******************************************************************/
int UART_recv(int fd,char *rbuf,int rbuf_len,int timeout)
{      
    int                   retval;
    fd_set                rset;
    struct timeval        time_out;
       
    if(!rbuf || rbuf_len <= 0)
    {  
        printf("%s,Invalid parameter.\n",__func__);
        return -1;
    }  

    if(timeout) //指定延时等待
    {    
        time_out.tv_sec = (time_t)(timeout / 1000);
        time_out.tv_usec = 0;

        FD_ZERO(&rset);
        FD_SET(fd,&rset);

        retval = select(fd + 1,&rset,NULL,NULL,&time_out);
        if(retval < 0)
        {
            printf("%s,Select failed:%s\n",strerror(errno));
            return -2;
        }

        else if(0 == retval)
        {
            printf("Time Out.\n");
            return 0;
        }

    }

    usleep(1000);

    retval = read(fd,rbuf,rbuf_len);
    if( retval <= 0)
    {
        printf("%s,Read failed:%s\n",__func__,strerror(errno));
        return -3;
    }

    return retval;
                         
}     
/********************************************************************
 * 名称：                  UART_Send
 * 功能：                  发送数据
 * 入口参数：        fd                  :文件描述符
 *                              send_buf    :存放串口发送数据
 *                              data_len    :一帧数据的个数
 * 出口参数：        正确返回为1，错误返回为0
 *******************************************************************/

int UART_send(int fd,char *sbuf,int sbuf_len)
{
    char     *ptr,*end;
    int       retval;

    if( !sbuf || sbuf_len <= 0)
    {
        printf("%s,Invalid parameter.\n",__func__);
        return -1;
    }

    if(sbuf_len > max_send_len)
    {
        ptr = sbuf;
        end = sbuf + sbuf_len;

        do
        {
            if(max_send_len < (end - ptr))
            {
                retval = write(fd,ptr,max_send_len);
                if(retval <= 0 || retval != max_send_len)
                {
                    printf("Write to com port[%d] failed:%s\n",fd,strerror(errno));
                    return -2;
                }
           
                ptr += max_send_len;
            }
            else 
            {
                retval = write(fd,ptr,(end - ptr));
                if(retval <= 0 || retval != (end - ptr))
                {
                    printf("Write to com port[%d] failed:%s\n",fd,strerror(errno));
                    return -3;
                }

                ptr += (end - ptr);
            }
        }while(end > ptr);
       
    }  
       
    else 
    {  
        retval = write(fd,sbuf,sbuf_len);
        if(retval <= 0 || retval != sbuf_len)
        {
            printf("Write to com port[[%d] failed:%s\n",fd,strerror(errno));
            return -4;
        }
    }  
       
    return retval;
}      

