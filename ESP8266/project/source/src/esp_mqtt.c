/*********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  esp_mqtt.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(14/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "14/08/20 17:53:58"
 *                 
 ********************************************************************************/

#include "esp_mqtt.h"

/***********************************************************
 * function :      测试esp8266 是否可以正常工作
 * name ：         check_esp
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int check_esp(comport_t *comport) 
{

    char r_buf[256];

    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+PING=\"baidu.com\"\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -3;
    }
    printf("Send command 'AT+CIFSR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));


    return 0;

}


/***********************************************************
 * function :      esp8266 加入路由器
 * name ：         join_route
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int join_route(comport_t *comport)
{

    char r_buf[256];

    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));



    return 0;
}



/***********************************************************
 * function :      esp8266连接腾讯云服务器
 * name ：         join_mqtt
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int join_mqtt(comport_t *comport)
{

    char r_buf[256];

    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));


    return 0;
}

/***********************************************************
 * function :      订阅腾讯云主题
 * name ：         mqtt_sub
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int mqtt_sub(comport_t *comport)
{

    char r_buf[256];

    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));



    return 0;
}

/***********************************************************
 * function :      esp8266上报消息给腾讯云
 * name ：         mqtt_pub
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int mqtt_pub(comport_t *comport)
{

    char r_buf[256];

    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));



    return 0;
}


/***********************************************************
 * function :      断开与腾讯云的连接
 * name ：         check_esp
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int disconn_mqtt(comport_t *comport)
{

    char r_buf[256];

    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));



    return 0;
}
