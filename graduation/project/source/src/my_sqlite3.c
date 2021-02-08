/*********************************************************************************
 *      Copyright:  (C) 2020 BIG WORLD
 *                  All rights reserved.
 *
 *       Filename:  sqlites.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(2020年04月05日)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年04月05日 12时03分52秒"
 *                 
 ********************************************************************************/
#include <netinet/in.h>
#include "my_sqlite3.h"

#include <time.h>
#include <sys/stat.h>
#include <dirent.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include <signal.h>


int callback(void *para, int f_num, char **f_value, char **f_name)
{

        int i = 0;
        for (i = 0; i < f_num; i++)
        {
            printf("%-11s|", f_value[i]);
        }
        putchar(10);
        return 0;
}



void do_insert_query(sqlite3 *db, char *a, char *b, char *c)
{
        char        sql2[128] = {};
        char        *errmsg;
        sprintf(sql2, "create table %s (id char, datetime char, wendu char);", a);

        if(sqlite3_exec(db, sql2, NULL, NULL, &errmsg) != SQLITE_OK)
        {
                printf("%s\n",errmsg);
        }
//        printf("create table success\n");



        char        sql3[128] = {};
        sprintf(sql3, "insert into '%s' values('%s','%s', '%s');", a, a, b, c);

        if(sqlite3_exec(db, sql3, NULL, NULL, &errmsg) != SQLITE_OK)
        {
                printf("%s\n", errmsg);
        }


        char sql4[128] = {};
        sprintf(sql4, "select * from '%s';", a);
        if(sqlite3_exec(db, sql4, callback, NULL, &errmsg) != SQLITE_OK)
        {
                printf("%s\n", errmsg);
        }

        return;
}

