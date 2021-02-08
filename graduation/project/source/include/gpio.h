/********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  gpio.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(16/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "16/08/20 14:42:27"
 *                 
 ********************************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include "bcm2835.h"

#define R_PIN 13
#define Y_PIN 19
#define G_PIN 26

#define turn_light_on(x) bcm2835_gpio_write(x, HIGH) 
#define turn_light_off(x) bcm2835_gpio_write(x, LOW)

void GPIO_init();

#endif 
