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

#define R_PIN 13
#define Y_PIN 19
#define G_PIN 26


#if 0
{

    bcm2835_init();                  //initialise bcm2835 gpio library
    OLED_Init();                         //initialise OLED module  
    OLED_Clear(0);               //clear OLED screen(black)
    while(1) 
    {
        show_time();
        int result = 1 ;
        show_connect_result(result);
        show_msg();
        //  delay_ms(5000);
        OLED_Clear(0);

    }
}
#endif 

int check_esp(comport_t *comport) 
{

    char s_buf[256];
    if( send_atcmd(comport, "AT\r\n", 500, AT_EXPSTR, AT_ERRSTR,s_buf, sizeof(s_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -3;
    }
    printf("Send command 'AT' got reply: %s\n",s_buf);

//    memset(s_buf, 0, sizeof(s_buf));
    if( send_atcmd(comport, "AT+GMR\r\n", 500, AT_EXPSTR, AT_ERRSTR,s_buf, sizeof(s_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -3;
    }
    printf("Send command 'AT+GMR' got reply: %s\n",s_buf);

  //  memset(s_buf, 0, sizeof(s_buf));
    if( send_atcmd(comport, "AT+CIFSR\r\n", 500, AT_EXPSTR, AT_ERRSTR,s_buf, sizeof(s_buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -3;
    }
    printf("Send command 'AT+CIFSR' got reply: %s\n",s_buf);


    return 1;
}


int main (int argc, char *argv[])
{ 
    logger_t          logger;

    comport_t        *comport = NULL;
    char             *dev_name = "/dev/ttyUSB0";
    char             *settings = "8N1N";
    unsigned long     baudrate = 115200;

    char              buf[256];

//gpio init
    if (!bcm2835_init())
                return 1;

//设置引脚为输出状态
    bcm2835_gpio_fsel(R_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(G_PIN, BCM2835_GPIO_FSEL_OUTP);

    if ( logger_init(&logger, DBG_LOG_FILE, LOG_LEVEL_NRML, LOG_ROLLBACK_NONE) || logger_open() )
    {
        printf("initialise logger system failure\n");
        return -1;
    }
    log_nrml("logger system start ok\n");

    if( !(comport=comport_init(dev_name, baudrate, settings)) || comport_open(comport)<0 )
    {
        log_err("initialise comport[%s] for AT command test failure: %s\n", dev_name);
        return -2;
    }
    log_nrml("open comport[%s] for AT command ok\n", comport->dev_name);
    check_esp(comport );

    comport_term(comport);
    logger_term();

    return 0; 
} 


