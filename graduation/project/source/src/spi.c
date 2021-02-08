#include "spi.h"

/*****************************************************************************
 * @name       :void SPI_WriteByte(uint8_t byte)
 * @date       :2018-08-27 
 * @function   :Write a byte of data using RaspberryPi hardware SPI
 * @parameters :Byte:Data to be written          
 * @retvalue   :Data received by the bus
******************************************************************************/
void SPI_WriteByte(uint8_t byte)
{
	bcm2835_spi_transfer(byte);		
}

/*****************************************************************************
 * @name       :void SPI_SetSpeed(uint16_t divider)
 * @date       :2018-08-27 
 * @function   :Set hardware SPI Speed
 * @parameters :divider:the speed value
 * @retvalue   :None
******************************************************************************/
void SPI_SetSpeed(uint16_t divider)
{
	bcm2835_spi_setClockDivider(divider);
} 

/*****************************************************************************
 * @name       :void SPI_Init(void)	
 * @date       :2018-08-27
 * @function   :Initialize the RaspberryPi hardware SPI
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void SPI_Init(void)	
{
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);     //The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                  //The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_4);    //The default
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);                     //The default
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);     //the default   
}
