/* ********************************************************************************
 *       Copyright:  (C) 2020 big world
 *                   All rights reserved.
 *            
 *        Filename:  main.c
 *     Description:  This file 
 *                                     
 *         Version:  1.0.0(14/08/20)
 *          Author:  xuxinhua <1411032148@qq.com>
 *       ChangeLog:  1, Release initial version on "14/08/20 15:17:40"
 *                                     
 **************************************************************************************/
#include <bcm2835.h>
#include "oled.h"
#include "menu_show.h"
#include "delay.h"
int main(void)
{	


	bcm2835_init(); 	         //initialise bcm2835 gpio library	
	OLED_Init();			     //initialise OLED module  
	OLED_Clear(0);               //clear OLED screen(black)
	while(1) 
	{	
//        show_time();
        OLED_Clear(0);
       int result = 1 ;
        show_connect_result(result);
        show_msg();
         delay_ms(5000);
        OLED_Clear(0);

	}
}
