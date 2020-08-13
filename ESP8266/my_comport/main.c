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

#define FALSE  -1
#define TRUE   0


int g_stop = 0;

void sig_handler(int sig_num)
{
    if(sig_num == SIGINT)
        g_stop = 1;
}

void adjust_buf(char* buf);

void help_information();

void printf_attr(UART_attr *attr)
{
    printf("attr is %d, %d, %d,%c, %d\n ", attr->flow_ctrl,attr->speed,attr->databits,attr->parity,attr->stopbits);
    return ;
}


//主函数
int main(int argc, char *argv[])
{
    int              retval;
    int              ch;
    char             sbuf[128] = {0};
    char             rbuf[1024] = {0};
    fd_set           rset;
    UART_attr*          attr = NULL;
    char 		port[128];
    int 	fd;

    struct option    options[] = {
        {"help",no_argument,NULL,'h'},
        {"baudrate",required_argument,NULL,'b'},
        {"databits",required_argument,NULL,'d'},
        {"parity",required_argument,NULL,'p'},
        {"stopbits",required_argument,NULL,'s'},
        {"name",required_argument,NULL,'n'},
        {NULL,0,NULL,0}
    };

    attr = (UART_attr*)malloc(sizeof(UART_attr));
    memset(attr,0,sizeof(UART_attr));
    
    while((ch = getopt_long(argc,argv,"hb:d:p:s:n:",options,NULL)) != -1)
    {
        switch(ch)
        {
            case 'h':
                help_information();
                return 0;

            case 'b':
                attr->speed = atoi(optarg);
                break;

            case 'd':
                attr->databits = atoi(optarg);
                break;

            case 'p':
                attr->parity = optarg[0];
                break;

            case 's':
                attr->stopbits = atoi(optarg);
                break;

            case 'n':
                strncpy(port ,optarg,128);
                break;
                
        }
    }

    if(strlen(port) == 0)
    {
        printf("Parameter warning:\n");
        printf("\tAt least need to enter the serial port name,You can specify the serial port name with -n.\n");
        return 0;
    }

    if((fd=UART_open(fd, port)) < 0)
    {
        printf("Open %s failed!\n",port);
     //   return -1;
    }

    printf("open %d(%s) successfully\n", fd, port);
/*    初始化  */
    UART_set(fd,attr);
    printf_attr(attr);
    signal(SIGINT,sig_handler);
    
    fflush(stdin);
    printf("Start to communicate with ESP8266......\n");
#if 0
    char   sbuf[128] = "AT+GMR\r\n";
    UART_send(fd,sbuf,sizeof(sbuf));
    sleep(2);
    UART_recv(fd,rbuf,sizeof(rbuf),0);
    printf("receive : %s", rbuf);
#endif 


#if 1
    while(!g_stop)
    {
       // printf("enter while \n");
        FD_ZERO(&rset);
        FD_SET(STDIN_FILENO,&rset);
        FD_SET(fd,&rset);
       // printf("start to select\n");
        /*  使用多路复用监听标准输入和串口fd */
        retval = select(fd + 1,&rset,NULL,NULL,NULL);
        if(retval < 0)
        {
            printf("Program exit......\n");
            break;
        }

        if(retval == 0)
        {
            printf("Time Out.\n");
            goto cleanup;
        }

        if(FD_ISSET(STDIN_FILENO,&rset))
        {
            memset(sbuf,0,sizeof(sbuf));

            /* 从标准输入读取命令 */
            fgets(sbuf,sizeof(sbuf),stdin);
            /* 处理要发送的数据 */  
            adjust_buf(sbuf);
            
            if(UART_send(fd,sbuf,strlen(sbuf)) < 0)
            {
                printf("Write failed.\n");
                goto cleanup;
            }
            fflush(stdin);

        }

        if(FD_ISSET(fd,&rset))
        {
            memset(rbuf,0,sizeof(rbuf));

            retval = UART_recv(fd,rbuf,sizeof(rbuf),0);
            if(retval <= 0)
            {
                printf("Read failed:%s\n",strerror(errno));
                break;
            }
            printf("%s\n", rbuf);
            fflush(stdout);
        }
        
    }
#endif

cleanup:
    UART_close(fd);

    return 0;

}

void adjust_buf(char *buf)
{
    int i = strlen(buf);
    strcpy(&buf[i-1],"\r\n\r\n");
}

void help_information()
{
    printf("\t-b   Set BaudRate\n");
    printf("\t-d   Set Databits\n");
    printf("\t-p   Set Parity,0 for no parity,1 for Odd parity,2 for Evev parity\n");
    printf("\t-s   Set StopBits\n");
    printf("\t-n   Set the name of the serial port you want to use\n");
    printf("\t     Ctrl + c to exit the program\n");
    printf("\n\tIf you do not specify parameters,The default parameters of the program are as follows:\n");
    printf("\tBaudRate: 1115200\n\tDatabits: 8bits\n\tParity:   no parity\n\tStopBits: 1bits\n");
}

