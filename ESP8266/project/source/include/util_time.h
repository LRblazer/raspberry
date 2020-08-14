/********************************************************************************
 *      Copyright:  (C) 2020 LingYun IoT System Studio
 *                  All rights reserved.
 *
 *       Filename:  util_time.h
 *    Description:  This head file is system time, timer API
 *
 *        Version:  1.0.0(07/23/2020)
 *         Author:  Guo Wenxue <guowenxue@gmail.com>
 *      ChangeLog:  1, Release initial version on "07/23/2020 07:46:37 AM"
 *                 
 ********************************************************************************/
#ifndef __UTIL_TIME_H_
#define __UTIL_TIME_H_

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

typedef struct date_time_s
{
    int year;
    int month;
    int day;
    int hour; 
    int minute;
    int second;
    int dayofweek;
} date_time_t;

/* sleep for micro second */
static inline void msleep(unsigned long ms)
{
    struct timespec  timeout; 
    unsigned long    tmp;

    timeout.tv_sec = ms / 1000;
    if (timeout.tv_sec == 0)
    {
        tmp = ms * 10000;
        timeout.tv_nsec = tmp * 100;
    }
    else
    {
        timeout.tv_nsec = 0;
    } 
    
    nanosleep(&timeout, 0);
}

/* call gettimeofday() to get current micro second */
static inline unsigned long time_now()
{
    struct timeval            now; 

    gettimeofday(&now, 0);

    return (now.tv_sec*1000) + (now.tv_usec/1000);
}

/*
* These inlines deal with timer wrapping correctly. You are
* strongly encouraged to use them
* 1. Because people otherwise forget
* 2. Because if the timer wrap changes in future you won't have to
* alter your driver code.
*
* time_after(a,b) returns true if the time a is after time b.
*
* Do this with "<0" and ">=0" to only test the sign of the result. A
* good compiler would generate better code (and a really good compiler
* wouldn't care). Gcc is currently neither.
*/

#define typecheck(type,x) \
({      type __dummy; \
        typeof(x) __dummy2; \
        (void)(&__dummy == &__dummy2); \
        1; \
})

#define time_after(a,b) \
(typecheck(unsigned long, a) && typecheck(unsigned long, b) && ((long)(b) - (long)(a) < 0))
#define time_before(a,b) time_after(b,a)
 
#define time_after_eq(a,b) \
(typecheck(unsigned long, a) && typecheck(unsigned long, b) && ((long)(a) - (long)(b) >= 0))
#define time_before_eq(a,b) time_after_eq(b,a)

/* Same as above, but does so with platform independent 64bit types.
 * These must be used when utilizing jiffies_64 (i.e. return value of
 * get_jiffies_64() */
#define time_after64(a,b) \
    (typecheck(__u64, a) && typecheck(__u64, b) && ((__s64)(b) - (__s64)(a) < 0))
#define time_before64(a,b)  time_after64(b,a)
 
#define time_after_eq64(a,b) \
    (typecheck(__u64, a) && typecheck(__u64, b) && ((__s64)(a) - (__s64)(b) >= 0))
#define time_before_eq64(a,b)   time_after_eq64(b,a)

               
static inline void get_sys_time(date_time_t *date)
{
    time_t now = time(NULL);
    struct tm *tnow = localtime(&now); 
    
    memset(date, 0, sizeof(*date)); 
    date->year = 1900 + tnow->tm_year;
    date->month = 1 + tnow->tm_mon;
    date->day = tnow->tm_mday;

    date->hour = tnow->tm_hour;
    date->minute = tnow->tm_min;
    date->second = tnow->tm_sec; 
    date->dayofweek = tnow->tm_wday; 
    return;
}

static inline int get_rtc_time(date_time_t *date)
{
    int                 rv, fd = -1;
    struct rtc_time     rtc_tm;  

    memset(date, 0, sizeof(*date));

    if ((fd=open("/dev/rtc0", O_RDONLY)) < 0)
        return -1;

    if((rv=ioctl(fd, RTC_RD_TIME, &rtc_tm)) < 0)
        return -2;

    date->year = 1900 + rtc_tm.tm_year;
    date->month = 1 + rtc_tm.tm_mon;
    date->day = rtc_tm.tm_mday;

    date->hour = rtc_tm.tm_hour;
    date->minute = rtc_tm.tm_min;
    date->second = rtc_tm.tm_sec;
    date->dayofweek = rtc_tm.tm_wday;

    close(fd);

    return 0;
}

#endif
