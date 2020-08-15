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
#include "delay.h"

/***********************************************************
 * function :      测试esp8266 是否可以正常工作
 * name ：         check_esp
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int check_esp(comport_t *comport) 
{

    char r_buf[256];

    printf("check esp\n");
    //AT 
    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -1;
    }
    printf("Send command 'AT' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    //AT+GMR 版本信息
    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+GMR got error\n");
        return -2;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    //AT+CIFSR 查询原先连接的AP信息
    if( send_atcmd(comport, "AT+CIFSR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+CIFSR got error\n");
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


    printf("join route\n");

    //AT+CWQAP 退出原先的路由器    
    if( send_atcmd(comport, "AT+CWQAP\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+CWQAP got error\n");
        return -1;
    }
    printf("Send command 'AT+CWQAP' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    //AT+CWJAP 加入新的路由器
    if( send_atcmd(comport, "AT+CWJAP=\"xxs\",\"xuxinhua666\"\r\n", 5000, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+CWJAP got error\n");
        return -2;
    }
    printf("Send command 'AT+CWJAP' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    //AT+PING="baidu.com" 测试是否可以联网
    if( send_atcmd(comport, "AT+PING=\"baidu.com\"\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+PING got error\n");
        return -3;
    }
    printf("Send command 'AT+PING' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));


    //AT+CIFSR 查询新连接的AP信息
    if( send_atcmd(comport, "AT+CIFSR\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+CIFSR got error\n");
        return -4;
    }
    printf("Send command 'AT+CIFSR' got reply: %s\n",r_buf);

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


    //AT+TCDEVINFOSET 设置平台信息
    if( send_atcmd(comport, "AT+TCDEVINFOSET=1,\"KL71ETP5T1\",\"dev_temp\",\"M/LL3AQfylFNUwG5FryrbA==\"\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCDEVINFOSET got error\n");
        return -1;
    }
    printf("Send command 'AT+TCDEVINFOSET' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    //AT+TCDEVINFOSET?   查询上面配置的信息
    if( send_atcmd(comport, "AT+TCDEVINFOSET?\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCDEVINFOSET? got error\n");
        return -2;
    }
    printf("Send command 'AT+TCDEVINFOSET?' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    //AT+TCMQTTCONN=1,5000,240,1,1  //连接腾讯云
    if( send_atcmd(comport, "AT+TCMQTTCONN=1,5000,240,1,1\r\n", 5000, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTCONN=1,5000,240,1,1 got error\n");
        return -3;
    }
    printf("Send command 'AT+TCMQTTCONN=1,5000,240,1,1' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    delay_ms(3000);

    //AT+TCMQTTSTATE?   查询是否连接成功
    if( send_atcmd(comport, "AT+TCMQTTSTATE?\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTSTATE? got error\n");
        return -2;
    }
    printf("Send command 'AT+TCMQTTSTATE?' got reply: %s\n",r_buf);
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

    delay_ms(3000);
    //AT+TCMQTTSUB="$thing/down/property/KL71ETP5T1/dev_temp",0
    if( send_atcmd(comport, "AT+TCMQTTSUB=\"$thing/down/property/KL71ETP5T1/dev_temp\",0\r\n", 2000, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTSUB got error\n");
        return -1;
    }
    printf("Send command 'AT+TCMQTTSUB' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    delay_ms(3000);
    return 0;
}

/***********************************************************
 * function :      esp8266上报消息给腾讯云
 * name ：         mqtt_pub
 * parameters :    自定义的结构体
 * retvalue ：     执行正确返回0
 ***********************************************************/
int mqtt_pub(comport_t *comport, float *temp, uint8_t state)
{

    char r_buf[512];
    char s_buf[512];


     snprintf( s_buf, sizeof(s_buf),"AT+TCMQTTPUB=\"$thing/up/property/KL71ETP5T1/dev_temp\",0,\"{\\\"method\\\": \\\"report\\\"\\,\\\"clientToken\\\": \\\"123\\\"\\,\\\"timestamp\\\": 1212121221\\,\\\"params\\\": {\\\"Temperature\\\": %.2f\\,\\\"light\\\": %d}}\"\r\n\0",*temp, state );


    printf("s_buf:%s\n", s_buf);

    delay_ms(3000);
    //AT+TCMQTTPUB="$thing/up/property/KL71ETP5T1/dev_temp",0,"{\"method\": \"report\"\,\"clientToken\": \"123\"\,\"timestamp\": 1212121221\,\"params\": {\"Temperature\": 32\,\"light\": 1}}"
    if( send_atcmd(comport,s_buf , 2000, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTPUB got error\n");
        return -1;
    }
    printf("Send command 'AT+TCMQTTPUB' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    delay_ms(3000);
    delay_ms(3000);
    delay_ms(3000);
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


    delay_ms(5000);

    //AT+TCMQTTUNSUP 取消订阅
    if( send_atcmd(comport, "AT+TCMQTTUNSUB=\"$thing/down/property/KL71ETP5T1/dev_temp\"\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTUNSUB got error\n");
        return -2;
    }
    printf("Send command 'AT+TCMQTTUNSUP' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));
  
    delay_ms(5000);
   
    //AT+TCMQTTDISCONN 断开mqtt服务器连接
    if( send_atcmd(comport, "AT+TCMQTTDISCONN\r\n", 2500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTDISCONN got error\n");
        return -1;
    }
    printf("Send command 'AT+TCMQTTDISCONN' got reply: %s\n",r_buf);

    //AT+TCMQTTSTATE?   查询是否连接成功
    if( send_atcmd(comport, "AT+TCMQTTSTATE?\r\n", 500, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCMQTTSTATE? got error\n");
        return -2;
    }
    printf("Send command 'AT+TCMQTTSTATE?' got reply: %s\n",r_buf);

#if 0
    //AT+TCRESTORE     抹除原来配置的mqtt平台信息
    if( send_atcmd(comport, "AT+TCRESTORE\r\n", 10000, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+TCRESTORE got error\n");
        return -3;
    }
    printf("Send command 'AT+TCRESTORE' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

#endif
    //AT+CWQAP 退出原先的路由器    
    if( send_atcmd(comport, "AT+CWQAP\r\n", 8000, AT_EXPSTR, AT_ERRSTR,r_buf, sizeof(r_buf)) <= 0 )
    {
        log_err("Send command AT+CWQAP got error\n");
        return -4;
    }
    printf("Send command 'AT+CWQAP' got reply: %s\n",r_buf);
    memset(r_buf, 0, sizeof(r_buf));

    return 0;
}
