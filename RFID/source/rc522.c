/*********************************************************************************
 *      Copyright:  (C) 2020 BIG WORLD
 *                  All rights reserved.
 *
 *       Filename:  rc522.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年07月22日)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年07月22日 22时03分30秒"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include "rc522.h"
#include <bcm2835.h>
#include <stdlib.h>


void RFID_init()
{
	bcm2835_spi_begin();
 
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
 
	writeMFRC522(CommandReg, PCD_RESETPHASE);
	writeMFRC522(TModeReg, 0x8D);
	writeMFRC522(TPrescalerReg, 0x3E);
	writeMFRC522(TReloadRegL, 30);
	writeMFRC522(TReloadRegH, 0);
	writeMFRC522(TxAutoReg, 0x40);   //100% ASK
	writeMFRC522(ModeReg, 0x3D);  // CRC valor inicial de 0x6363
	antennaOn();   //打开天线
}




/////////////////////////////////////////////////////////////////////
//功    能：写RC522寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
/////////////////////////////////////////////////////////////////////
void writeMFRC522(unsigned char Address, unsigned char value)
{
    char buff[2];
    buff[0] = (char)((Address<<1)&0x7E);
    buff[1] = (char)value;
    
    bcm2835_spi_transfern(buff,2); 
}





/////////////////////////////////////////////////////////////////////
//功    能：读RC522寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
/////////////////////////////////////////////////////////////////////
unsigned char readMFRC522(unsigned char Address)
{
    char buff[2];
    buff[0] = ((Address<<1)&0x7E)|0x80;
    bcm2835_spi_transfern(buff,2);
    return (uint8_t)buff[1];
}




/////////////////////////////////////////////////////////////////////
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
/////////////////////////////////////////////////////////////////////
void setBitMask(unsigned char reg, unsigned char mask)
{
	unsigned char tmp;
	tmp = readMFRC522(reg);
	writeMFRC522(reg, tmp | mask);
}




/////////////////////////////////////////////////////////////////////
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
/////////////////////////////////////////////////////////////////////
void clearBitMask(unsigned char reg, unsigned char mask)
{
	unsigned char tmp;
	tmp = readMFRC522(reg);
	writeMFRC522(reg, tmp & (~mask));
}




/////////////////////////////////////////////////////////////////////
//开启天线  
//每次启动或关闭天线发射之间应至少有1ms的间隔
/////////////////////////////////////////////////////////////////////
void antennaOn(void)
{
	unsigned char temp;
	temp = readMFRC522(TxControlReg);
	if(!(temp & 0x03))
	{
		setBitMask(TxControlReg, 0x03);
	}
}




/////////////////////////////////////////////////////////////////////
//关闭天线
/////////////////////////////////////////////////////////////////////
void antennaOff(void)
{
	unsigned char temp;
	temp = readMFRC522(TxControlReg);
	if(!(temp & 0x03))
	{
		clearBitMask(TxControlReg, 0x03);
	}
}




/////////////////////////////////////////////////////////////////////
//用MF522计算CRC16函数
/////////////////////////////////////////////////////////////////////
void calculateCRC(unsigned char *pIndata, unsigned char len, unsigned char *pOutData)
{
	unsigned char i, n;
	clearBitMask(DivIrqReg, 0x04);
	setBitMask(FIFOLevelReg, 0x80);
	for (i=0; i<len;i++)
	{
		writeMFRC522(FIFODataReg, *(pIndata+i));
	}
	writeMFRC522(CommandReg, PCD_CALCCRC);
	i = 0xFF;
	do
	{
		n = readMFRC522(DivIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x04));
	pOutData[0] = readMFRC522(CRCResultRegL);
	pOutData[1] = readMFRC522(CRCResultRegM);
}


/////////////////////////////////////////////////////////////////////
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          sendData[IN]:通过RC522发送到卡片的数据
//          sendLen[IN]:发送数据的字节长度
//          backData[OUT]:接收到的卡片返回数据
//          backLen[OUT]:返回数据的位长度
/////////////////////////////////////////////////////////////////////
unsigned char MFRC522ToCard(unsigned char command, unsigned char *sendData, unsigned char sendLen, unsigned char *backData, unsigned int *backLen)
{
	unsigned char status = MI_ERR;
	unsigned char irqEn = 0x00;
	unsigned char waitIRq = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;
	switch(command)
	{
		case PCD_AUTHENT:     //认证卡密
		{
			irqEn = 0x12;
			waitIRq = 0x10;
			break;
		}
		case PCD_TRANSCEIVE:  //发送FIFO中数据
		{
			irqEn = 0x77;
			waitIRq = 0x30;
			break;
		}
		default:
			break;
  }
	writeMFRC522(CommIEnReg, irqEn|0x80); //允许中断请求
	clearBitMask(CommIrqReg, 0x80);       //清除所有中断请求位
	setBitMask(FIFOLevelReg, 0x80);       //FlushBuffer=1, FIFO初始化
	writeMFRC522(CommandReg, PCD_IDLE);   //无动作，取消当前命令
	//向FIFO中写入数据
	for (i=0; i<sendLen;i++)
	{
		writeMFRC522(FIFODataReg, sendData[i]);
	}
	//执行命令
	writeMFRC522(CommandReg, command);
	if(command == PCD_TRANSCEIVE)
    setBitMask(BitFramingReg, 0x80);
	//等待接收数据完成
	i = 2000; //i根据时钟频率调整，操作M1卡最大等待时间25ms
	do
	{
		n = readMFRC522(CommIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitIRq));
	clearBitMask(BitFramingReg, 0x80);
	if(i != 0)
	{
		if(!(readMFRC522(ErrorReg) & 0x1B))
		{
			status = MI_OK;
			if(n & irqEn & 0x01)
				status = MI_NOTAGERR;
			if(command == PCD_TRANSCEIVE)
			{
				n = readMFRC522(FIFOLevelReg);
				lastBits = readMFRC522(ControlReg) & 0x07;
				if(lastBits)
					*backLen = (n-1)*8 + lastBits;
				else
					*backLen = n*8;
				if(n == 0)
					n = 1;
				if(n > MAX_LEN)
					n = MAX_LEN;
				//读取FIFO中接收到的数据
				for (i=0; i<n;i++)
					backData[i] = readMFRC522(FIFODataReg);
			}
		}
		else
			status = MI_ERR;
	}
	return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：寻卡
//参数说明: reqMode[IN]:寻卡方式
//                0x52 = 寻感应区内所有符合14443A标准的卡
//                0x26 = 寻未进入休眠状态的卡
//          TagType[OUT]：卡片类型代码
//                0x4400 = Mifare_UltraLight
//                0x0400 = Mifare_One(S50)
//                0x0200 = Mifare_One(S70)
//                0x0800 = Mifare_Pro(X)
//                0x4403 = Mifare_DESFire
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
unsigned char findCard(unsigned char reqMode, unsigned char *TagType)
{
	unsigned char status;
	unsigned int backBits;
	writeMFRC522(BitFramingReg, 0x07);
	TagType[0] = reqMode;
	status = MFRC522ToCard(PCD_TRANSCEIVE, TagType, 1, TagType, &backBits);
	if((status != MI_OK) || (backBits != 0x10))
		status = MI_ERR;
	return status;
}



/////////////////////////////////////////////////////////////////////
//功    能：防冲撞
//参数说明: pSnr[OUT]:卡片序列号，4字节
//返    回: 成功返回MI_OK
///////////////////////////////////////////////////////////////////// 
unsigned char anticoll(unsigned char *serNum)
{
	unsigned char status;
	unsigned char i;
	unsigned char serNumCheck=0;
	unsigned int unLen;
	clearBitMask(Status2Reg, 0x08);
	clearBitMask(CollReg,0x80);
	writeMFRC522(BitFramingReg, 0x00);
	serNum[0] = PICC_ANTICOLL;
	serNum[1] = 0x20;
	status = MFRC522ToCard(PCD_TRANSCEIVE, serNum, 2, serNum, &unLen);
	if(status == MI_OK)
	{
		//校验卡序列号
		for (i=0; i<4; i++)
		{
			*(serNum+i)  = serNum[i];
			serNumCheck ^= serNum[i];
		}
		if(serNumCheck != serNum[i])
		{
			status = MI_ERR;
		}
	}
	setBitMask(CollReg, 0x80);
	return status;
}


 /////////////////////////////////////////////////////////////////////
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
void RFID_halt()
{
	unsigned char status;
	unsigned int unLen;
	unsigned char buff[4];
	buff[0] = PICC_HALT;
	buff[1] = 0;
	calculateCRC(buff, 2, &buff[2]);
	status = MFRC522ToCard(PCD_TRANSCEIVE, buff, 4, buff,&unLen);
}
 
