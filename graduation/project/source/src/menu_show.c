/*********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  menu_show.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "11/08/20 16:23:48"
 *                 
 ********************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "oled.h"
#include "delay.h"
#include "gui.h"
#include "menu_show.h"
//#include "bmp.h"
#include "temp.h"
#include "get_time.h"
#include <string.h>
#include <bcm2835.h>


//在OLED屏幕上显示时间
int show_time(void)
{
    char buf_time[20] ;
    memset(buf_time, 0, sizeof(buf_time));
    get_time(buf_time);
    GUI_ShowString(0,0,"current time:",16,1);
    GUI_ShowString(0,20,buf_time,16,1);
    delay_ms(1000);
    OLED_Clear(0);

    return 0;
}


/************************************************************************
 * name :           show_msg
 * function :       在OLED上显示温度，esp8266连接情况，台灯黄灯的状态
 * parameters :     ds18b20获取的温度，和黄灯状态
 * retvalue ：      正确返回0
 *************************************************************************/
int show_msg(float *temp, uint8_t state)
{

 //   float temp =0;
    char  buf_temp[5];
 //   uint8_t state = bcm2835_gpio_lev(R_PIN);
  
    memset(buf_temp, 0, sizeof(buf_temp));
   
  //  get_temperature(&temp);
    snprintf(buf_temp,sizeof(buf_temp),"%.2f",*temp);
    
    GUI_DrawLine(0, 10, WIDTH-1, 10,1);
    GUI_DrawLine(WIDTH/2-1,11,WIDTH/2-1,HEIGHT-1,1);
    GUI_DrawLine(WIDTH/2-1,10+(HEIGHT-10)/2-1,WIDTH-1,10+(HEIGHT-10)/2-1,1);
   
    
    GUI_ShowString(14,HEIGHT-1-10,"Light",8,1);
    GUI_ShowString(WIDTH/2-1+2,13,"TEMP",8,1);
   
    GUI_DrawCircle(WIDTH-1-19, 25, 1,2);
    GUI_ShowString(WIDTH-1-14,20,"C",16,1);
   
    GUI_ShowString(WIDTH/2-1+9,20,buf_temp,16,1);
    GUI_ShowString(WIDTH/2-1+2,39,"state",8,1);
//check red light state 
    if( 1 == state )
    {
        GUI_ShowString(WIDTH/2-1+5,46,"on",16,1);
        GUI_FillCircle(27, 33, 1,8);
        GUI_DrawCircle(27, 33, 1,12);
    }else{
        GUI_ShowString(WIDTH/2-1+5,46,"off",16,1);
        GUI_FillCircle(27, 33, 0,8);
        GUI_DrawCircle(27, 33, 1,12);
    }


    delay_ms(10000);
    OLED_Clear(0);
    
#if 1
    char buf_time[20] ;
    memset(buf_time, 0, sizeof(buf_time));
    get_time(buf_time);
 //   GUI_ShowString(0,0,"current time:",16,1);
    GUI_ShowString(20,15,buf_time,16,1);
    
    delay_ms(1000);
    OLED_Clear(0);
#endif


    return 0;
}

//在屏幕显示是否成功连上mqtt
int show_connect_result(int connect_result)
{
    if( 0 == connect_result){
        GUI_ShowString(0,1,"connect successfully",8,1);
    }else{
        GUI_ShowString(0,1,"connect fail",8,1);
    }
    delay_ms(4000);
    OLED_Clear(0);
    return 0;
}


