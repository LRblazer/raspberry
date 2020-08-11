/********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  spi.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(11/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "11/08/20 14:27:05"
 *                 
 ********************************************************************************/

#ifndef _SPI_H_
#define _SPI_H_
#include <bcm2835.h>

//This program uses the hardware spi bus,
//the pin definition of colok signal,write signal and read signal can't be modified
//The SPI clock pin is fixed to 23(Physical pin,bcm coding is 11)
//The SPI write pin is fixed to 19(Physical pin,bcm coding is 10)
//The SPI read pin is not be used
 
void SPI_WriteByte(uint8_t byte);
void SPI_Init(void);
void SPI_SetSpeed(uint16_t divider);

#endif
