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
 //       show_time();
       int result = 1 ;
        show_connect_result(result);
        show_msg();
       //  delay_ms(5000);
        OLED_Clear(0);

	}
}
