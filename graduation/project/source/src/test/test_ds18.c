/*********************************************************************************
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
 ************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "temp.h"

#if 0
int   get_temperature(float *temp);  //函数声明



//test
int main(int argc, char *argv[])
{
	float temp;
	int   rv  ;

	rv = get_temperature(&temp);
	if( rv < 0 ) 
	{
		printf("ERROR:  get temperature failure %d\n", rv);   
		return -1;
	}

	printf(" temperature: %f\n", temp);
	return 0;
}


#endif
/*****************************************************************
* name :int get_temperature(float *temp)
* function: get temperature from ds18b20
* parameters ;
* retvalue:
******************************************************************/                                                
int get_temperature(float *temp)
{
	char            w1_path[64] = "/sys/bus/w1/devices/";
	char            chip_sn[20];
	char            buf[128];
	DIR             *dirp = NULL;
	struct  dirent  *direntp = NULL;
	int             fd = -1;
	char            *ptr = NULL;                                                      
	int             found = 0;


	dirp = opendir( w1_path );
	if( !dirp ) 
	{
		printf("opendir error %s\n", strerror(errno));
		return -1;
	}

	while ( (direntp = readdir(dirp)) != NULL) 
	{
		if( strstr(direntp->d_name, "28-") ) 
		{
			strncpy( chip_sn, direntp->d_name, sizeof(chip_sn));      
			found = 1;
		}
	}


	closedir(dirp);

	if( !found ) 
	{
		printf("can not find ds18b20 in %s\n", w1_path);   
		return -2;
	}


	strncat(w1_path, chip_sn, sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path, "/w1_slave", sizeof(w1_path)-strlen(w1_path));

	if( (fd = open(w1_path, O_RDONLY)) < 0 ) 
	{
		printf("open %s error : %s\n", w1_path, strerror(errno));   
		return -3;
	}


	if( read(fd, buf, sizeof(buf)) < 0 ) 
	{
		printf("read %s error : %s\n",w1_path, strerror(errno));    
		return -4;
	}

	ptr = strstr(buf, "t=");
	if( !ptr ) 
	{
		printf("error: can not get string \n");   
		return -5;
	}

	ptr += 2;

	*temp = atof(ptr)/1000;


	close(fd);
	return 0;

}





