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

#include "atcmd.h"
#include "logger.h"
#include <bcm2835.h>
#include "oled.h"
#include "menu_show.h"
#include "delay.h"
#include "comport.h"
#include "logger.h"
#include "atcmd.h"

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

int main (int argc, char *argv[])
{ 
    logger_t          logger;

    comport_t        *comport = NULL;
    char             *dev_name = "/dev/ttyUSB4";
    char             *settings = "8N1N";
    unsigned long     baudrate = 115200;

    char              buf[256];

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

    if( send_atcmd(comport, "AT\r", 500, AT_EXPSTR, AT_ERRSTR, buf, sizeof(buf)) <= 0 )
    {
        log_err("Send command AT got error\n");
        return -3;
    }
    printf("Send command 'AT' got reply: %s\n", buf);

    if( !send_atcmd_check_ok(comport, "ATE0\r", 500) )
    {
        log_nrml("Send command 'ATE0' got OK\n");
    }

    if( !send_atcmd_check_value(comport, "AT+CGMM\r", 500, buf, sizeof(buf) ) )
    {
        log_nrml("Send command 'AT+CGMM' got reply: %s\n", buf);
    }


    if( !send_atcmd_check_request(comport, "AT+CSQ\r", 500, buf, sizeof(buf)) )
    {
        log_nrml("Send command 'AT+CSQ' got reply: %s\n", buf);
    }

    if( !send_atcmd_check_request(comport, "AT+CREG?\r", 1000, buf, sizeof(buf)) )
    {
        log_nrml("Send command 'AT+CREG?' got reply: %s\n", buf);
    }


    if( !send_atcmd_check_request(comport, "AT+COPS?\r", 1000, buf, sizeof(buf)) )
    {
        log_nrml("Send command 'AT+CREG?' got reply: %s\n", buf);
    }

    comport_term(comport);
    logger_term();

    return 0; 
} 


