/*********************************************************************************
 *      Copyright:  (C) 2020 BIG WORLD
 *                  All rights reserved.
 *
 *       Filename:  test.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月22日)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月22日 22时05分34秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include "rc522.h"
#include <bcm2835.h>
#include <stdlib.h>


int main()
{
	unsigned char s;//读出2个字节数据TagType--卡片类型代码
	unsigned char id[10];//卡片序列号,4字节
	int i;
	if(!bcm2835_init())
		return -1;
	RFID_init();

	bcm2835_gpio_fsel(21, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(21, HIGH);
	while(1)
	{
		printf("going\n");
		if(findCard(0x52,&s)==MI_OK)
		{
			printf("hello boy!\n");
			if(anticoll(id)==MI_OK)
			{
				for(i=0;i<4;i++)
					printf("%d ",id[i]);
				printf("\n");
				bcm2835_gpio_write(21, LOW);
				system("mplayer welcome.wav");
				sleep(1);
				bcm2835_gpio_write(21,HIGH);
			}
			else printf("ERROR\n");
		}
		sleep(1);
	}
	bcm2835_spi_end();
	bcm2835_close();
	return 0;
}
