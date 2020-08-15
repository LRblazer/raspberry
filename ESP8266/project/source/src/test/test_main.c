/* ********************************************************************************
 *        Copyright:  (C) 2020 big world
 *                     All rights reserved.
 *    
 *         Filename:  main.c
 *      Description:  This file 
 *                        
 *          Version:  1.0.0(14/08/20)
 *           Author:  xuxinhua <1411032148@qq.com>
 *        ChangeLog:  1, Release initial version on "14/08/20 15:17:40"
 *                 
 **********************************************************************************/
#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "atcmd.h"
#include "logger.h"
#include <bcm2835.h>
#include "oled.h"
#include "menu_show.h"
#include "delay.h"
#include "comport.h"
#include "logger.h"
#include "atcmd.h"
#include "temp.h"
#include "get_time.h"
#include "esp_mqtt.h"

#define R_PIN 13
#define Y_PIN 19
#define G_PIN 26


int g_stop = 0;
void sig_exit(int sig_num)
{
    if(sig_num == SIGINT)
        g_stop = 1;
}

#if 0
void sig_alarm(int sig_num)
{
    alarm(50);
    printf("start pub \n");
                    
    if( mqtt_pub(comport,&temp, light_state) < 0 )
    {
        printf("pub unsuccessfully\n");
        disconn_mqtt(comport);
        exit 0;
    }
    
    printf("pub down\n");
}

#endif

int main (int argc, char *argv[])
{ 
    logger_t          logger;

    comport_t        *comport = NULL;
    char             *dev_name = "/dev/ttyUSB0";
    char             *settings = "8N1N";
    unsigned long     baudrate = 115200;

    char              buf[256];

    float temp = 0;
    int temp_rv = 0;

    uint8_t light_state = 2;

    int     mqtt_state = -1;
    
    signal(SIGINT,sig_exit);
   // signal(SIGALRM,sig_alarm);
    alarm(50);
    
    //gpio init
    if (!bcm2835_init())
        return 1;

    //初始化OLED
    OLED_Init();                         //initialise OLED module  
    OLED_Clear(0);               //clear OLED screen(black)


    //设置引脚为输出状态，即初始化GPIO
    bcm2835_gpio_fsel(R_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(G_PIN, BCM2835_GPIO_FSEL_OUTP);

    //日志初始化
    if ( logger_init(&logger, DBG_LOG_FILE, LOG_LEVEL_NRML, LOG_ROLLBACK_NONE) || logger_open() )
    {
        printf("initialise logger system failure\n");
        return -1;
    }
    log_nrml("logger system start ok\n");

    //串口初始化并打开串口
    if( !(comport=comport_init(dev_name, baudrate, settings)) || comport_open(comport)<0 )
    {
        log_err("initialise comport[%s] for AT command test failure: %s\n", dev_name);
        return -2;
    }
    log_nrml("open comport[%s] for AT command ok\n", comport->dev_name);

    //检测esp8266是否可以正常工作
    if(  check_esp(comport ) < 0 )
    {
        printf("check_esp unsuccessfully\n");
        return -3;
    }

    //esp8266 联网
    if(  join_route(comport) < 0 )
    {
        printf("join route unsuccessfully\n");
        return -4;
    }

    //esp8266 连接腾讯云MQTT服务器
    mqtt_state = join_mqtt(comport);
    if( mqtt_state < 0 ) 
    {
        printf("join mqtt unsuccessfully\n");
        disconn_mqtt(comport);
        return -5;
    }

    //在OLED上显示是否连接成功
    show_connect_result(mqtt_state);
    //连接成功亮绿灯
    bcm2835_gpio_write(G_PIN, HIGH);

    //订阅主题
    if( mqtt_sub(comport) < 0 ) 
    {
        printf("sub unsuccessfully\n");
        disconn_mqtt(comport);
        return -6;
    }

    while(!g_stop)
    {
        //获取黄灯状态和温度
        light_state = bcm2835_gpio_lev(Y_PIN);
        temp_rv  = get_temperature(&temp);

        if( temp_rv < 0 ) 
        {
                printf("ERROR:  get temperature failure %d\n", temp_rv);   
                return -1;
        }
        printf(" temperature: %f\n", temp);

        //如果温度高于30度，亮起红灯             
        if (temp > 30)
        {
            bcm2835_gpio_write(R_PIN, HIGH);         
        }
        //将黄灯的状态和温度显示在屏幕上
        show_msg(&temp, light_state);
        delay_ms(8000);


        //将采集的温度和台灯状态上发到腾讯云
#if 1 
    if( mqtt_pub(comport,&temp, light_state) < 0 )
    {
        printf("pub unsuccessfully\n");
        disconn_mqtt(comport);
        return -7;
    }
#endif 
    delay_ms(1000);
   }






    //退出程序前，断开与腾讯云的连接
    if( disconn_mqtt(comport) < 0 )
    {
        printf("disconn mqtt unsuccessfully\n");
        disconn_mqtt(comport);
        return -8;
    }


    bcm2835_gpio_write(R_PIN, LOW);
    bcm2835_gpio_write(Y_PIN, LOW);
    bcm2835_gpio_write(G_PIN, LOW);
    bcm2835_close();
   
    comport_term(comport);
    logger_term();

    return 0; 
} 


