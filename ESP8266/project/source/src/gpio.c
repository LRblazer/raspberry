/*********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  gpio.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(16/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "16/08/20 14:39:43"
 *                 
 ********************************************************************************/

#include "gpio.h"
//#include "bcm2835.h"


void GPIO_init()
{
    //设置引脚为输出状态，即初始化GPIO
    bcm2835_gpio_fsel(R_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(G_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("gpio init competely\n");
}


