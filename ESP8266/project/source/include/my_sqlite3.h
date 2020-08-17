/********************************************************************************
 *      Copyright:  (C) 2020 BIG WORLD
 *                  All rights reserved.
 *
 *       Filename:  head.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(2020年04月05日)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "2020年04月05日 11时58分07秒"
 *                 
 ********************************************************************************/

#ifndef _MY_SQLITE3_H_
#define _MY_SQLITE3_H_ 
#include"sqlite3.h"
extern  int     callback(void *para, int f_num, char **f_value, char **f_name);
extern  void    do_insert_query(sqlite3 *db, char *a, char *b, char *c);


#endif



