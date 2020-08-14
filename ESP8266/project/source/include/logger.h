/********************************************************************************
 *      Copyright:  (C) 2020 LingYun IoT System Studio
 *                  All rights reserved.
 *
 *       Filename:  logger.h
 *    Description:  This file is the linux infrastructural logger system library
 *
 *        Version:  1.0.0(08/08/2020~)
 *         Author:  Guo Wenxue <guowenxue@gmail.com>
 *      ChangeLog:  1, Release initial version on "08/08/2020 05:16:56 PM"
 *                 
 ********************************************************************************/

#ifndef  _LOGGER_H_
#define  _LOGGER_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/time.h>

#define LOG_VERSION_STR             "1.0.0"

#ifndef FILENAME_LEN
#define FILENAME_LEN                64
#endif

#define DBG_LOG_FILE                "stderr"  /*  Debug mode log file is console */ 

#define LOG_ROLLBACK_SIZE           512    /* Default rollback log size  */
#define LOG_ROLLBACK_NONE           0      /* Set rollback size to 0 will not rollback  */

#define DEFAULT_TIME_FORMAT         "%Y-%m-%d %H:%M:%S"
#define MAX_LOG_MESSAGE_LEN         0x1000

//#define DUMPLICATE_OUTPUT  /* Log to file and printf on console  */

enum
{
    LOG_LEVEL_DISB = 0,               /*  Disable "Debug" */
    LOG_LEVEL_FATAL,                  /*  Debug Level "Fatal" */
    LOG_LEVEL_ERROR,                  /*  Debug Level "ERROR" */
    LOG_LEVEL_WARN,                   /*  Debug Level "warnning" */
    LOG_LEVEL_NRML,                   /*  Debug Level "Normal" */
    LOG_LEVEL_DEBUG,                  /*  Debug Level "Debug" */
    LOG_LEVEL_INFO,                   /*  Debug Level "Information" */
    LOG_LEVEL_TRACE,                  /*  Debug Level "Trace" */
    LOG_LEVEL_MAX,
};



/* logger->flag definition */
#define FLAG_LOGGER_LEVEL_OPT           1<<0 /*  The log level is sepcified by the command option */

#define FLAG_LOGGER_CONSOLE             1<<1
#define FLAG_LOGGER_FILE                0<<1

typedef struct logger_s
{
    unsigned char      flag;
    char               file[FILENAME_LEN];
    int                level;
    int                size;

    FILE               *fp;
} logger_t;

extern char *log_str[];

/* log_size unit is KB */
extern int  logger_init(logger_t *logger, char *filename, int level, int log_size);
extern int  logger_open(void);
extern void logger_set_time_format(char *time_format);
extern int  logger_reopen(void);
extern void logger_close(void);
extern void logger_term(void);
extern void logger_raw(const char *fmt, ...);

extern void logger_min(int level, char *fmt, ...);
extern void logger_line(int level, char *file, int line, char *fmt, ...);
extern void logger_str(int level, const char *msg);
extern void logger_dump(int level, char *buf, int len);

#define LOG_FILE_LINE      /* Log the file and line */

#ifdef LOG_FILE_LINE
#define log_trace(fmt, ...) logger_line(LOG_LEVEL_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)  logger_line(LOG_LEVEL_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_dbg(fmt, ...)   logger_line(LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_nrml(fmt, ...)  logger_line(LOG_LEVEL_NRML,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...)  logger_line(LOG_LEVEL_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_err(fmt, ...)   logger_line(LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) logger_line(LOG_LEVEL_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#else
#define log_trace(fmt, ...) logger_min(LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...)  logger_min(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define log_dbg(fmt, ...)   logger_min(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define log_nrml(fmt, ...)  logger_min(LOG_LEVEL_NRML, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...)  logger_min(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define log_err(fmt, ...)   logger_min(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) logger_min(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#endif

#endif   /* ----- #ifndef _LOGGER_H_  ----- */

