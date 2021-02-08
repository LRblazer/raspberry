/*********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  delay.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "11/08/20 14:14:04"
 *                 
 ********************************************************************************/

#include "delay.h"

void delay_ms(uint32_t xms)
{
        bcm2835_delay(xms);
}
