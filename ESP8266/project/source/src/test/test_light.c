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
 *************************************************************************************/

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define R_PIN 13
#define Y_PIN 19
#define G_PIN 26

int k=10;
void sig_had()
{

    alarm(10);
    printf("k=%d\n", k);
    bcm2835_gpio_write(R_PIN, LOW);
    bcm2835_gpio_write(Y_PIN, LOW);
    bcm2835_gpio_write(G_PIN, LOW);
    bcm2835_delay(1500);
}




int main(int argc, char **argv)
{

    if (!bcm2835_init())
        return 1;

    //设置引脚为输出状态
    bcm2835_gpio_fsel(R_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(G_PIN, BCM2835_GPIO_FSEL_OUTP);


    signal(SIGALRM,sig_had);
    alarm(10);

    //    uint8_t k=0;
    //    k = bcm2835_gpio_lev(R_PIN);

    //blink
    while (1) {

        bcm2835_gpio_write(R_PIN, HIGH);
        bcm2835_gpio_write(Y_PIN, HIGH);
        bcm2835_gpio_write(G_PIN, HIGH);




        bcm2835_delay(1500);

        //        bcm2835_gpio_write(R_PIN, LOW);
        //      bcm2835_gpio_write(Y_PIN, LOW);
        //    bcm2835_gpio_write(G_PIN, LOW);




        //  bcm2835_delay(1500);
        //
    }
    //
    bcm2835_close();
    //
    return 0;


}
