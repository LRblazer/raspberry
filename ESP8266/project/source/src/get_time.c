#include <time.h>
#include <stdio.h>
#include "get_time.h"


/*********************************************
 * name:
 * function:
 * parameters:
 * retvalue:
 **********************************************/
void get_time(char *data_time)
{
	struct tm   *p;
	time_t      timep;
	// char        date_time[40];
	time(&timep);                    
	p =localtime(&timep);   

	snprintf(data_time, 50, "%d/%02d/%02d      %02d:%02d:%02d", (1900+p->tm_year),  (1+p->tm_mon), p->tm_mday, (7+p->tm_hour), p->tm_min, p->tm_sec);

	return ;
}
