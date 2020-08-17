/*********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  logger.c
 *    Description:  This file is the linux infrastructural logger system library
 *                 
 *        Version:  1.0.0(08/08/2020~)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "08/08/2020 04:24:01 PM"
 *                 
 ********************************************************************************/

#include "logger.h"

#define PRECISE_TIME_FACTOR 1000

static unsigned long log_rollback_size = LOG_ROLLBACK_NONE;

/* This library is not thread safe */
static logger_t *logger = NULL;

char *log_str[LOG_LEVEL_MAX + 1] = { "", "F", "E", "W", "N", "D", "I", "T", "M" };

#define LOG_TIME_FMT       "%Y-%m-%d %H:%M:%S"

static void log_signal_handler(int sig)
{
    if(!logger)
        return ;

    if (sig == SIGHUP)
    {
        signal(SIGHUP, log_signal_handler);
        log_fatal("SIGHUP received - reopenning log file [%s]", logger->file);
        logger_reopen();
    }
}

static void logger_banner(char *prefix)
{
    if(!logger)
        return ;

    fprintf(logger->fp, "%s log \"%s\" on level [%s] size [%lu] KiB, log system version %s\n",
            prefix, logger->file, log_str[logger->level], log_rollback_size / 1024, LOG_VERSION_STR);
#ifdef LOG_FILE_LINE
    fprintf(logger->fp, " [ Date ]    [ Time ]   [ Level ]  [ File/Line ]  [ Message ]\n");
#else
    fprintf(logger->fp, " [ Date ]    [ Time ]   [ Level ]  [ Message ]\n");
#endif
    fprintf(logger->fp, "-------------------------------------------------------------\n");
}

static void check_and_rollback(void)
{
    if(!logger)
        return ;

    if (log_rollback_size != LOG_ROLLBACK_NONE)
    {
        long _curOffset = ftell(logger->fp);

        if ((_curOffset != -1) && (_curOffset >= log_rollback_size))
        {
            char cmd[512];

            snprintf(cmd, sizeof(cmd), "cp -f %s %s.roll", logger->file, logger->file);
            system(cmd);

            if (-1 == fseek(logger->fp, 0L, SEEK_SET))
                fprintf(logger->fp, "log rollback fseek failed \n");

            rewind(logger->fp);

            truncate(logger->file, 0);
            logger_banner("Already rollback");
        }
    }
}


/* log_size unit is KB */
int logger_init(logger_t *log, char *log_file, int level, int log_size)
{
    if( !log )
    {
        printf("ERROR: Invalid input arguments\n");
        return -1;
    }

    if( log_file ) 
    {
        strncpy(log->file, log_file, FILENAME_LEN); 
        log->flag |= FLAG_LOGGER_FILE;
    }
    else
    {
        strncpy(log->file, DBG_LOG_FILE, FILENAME_LEN); 
        log->flag |= FLAG_LOGGER_CONSOLE;
    }

    log->level = level;
    log->size = log_size; 

    /* set static global $logger point to argument $log  */
    logger = log;

    return 0;
}

int logger_open(void)
{
    struct sigaction act;
    char *filemode;

    if(!logger)
    {
        printf("ERROR: logger not initialise\n");
        return -1;
    }

    log_rollback_size = logger->size <= 0 ? LOG_ROLLBACK_NONE : logger->size*1024;    /* Unit KiB */
        
    if ('\0' == logger->file[0])
    {
        printf("ERROR: Logger filename not set\n");
        return -1;
    }

    if (!strcmp(logger->file, DBG_LOG_FILE))
    {
        logger->fp = stderr;
        log_rollback_size = LOG_ROLLBACK_NONE;
        logger->flag |= FLAG_LOGGER_CONSOLE;
        goto OUT;
    }

    filemode = (log_rollback_size==LOG_ROLLBACK_NONE) ? "a+" : "w+";

    logger->fp = fopen(logger->file, filemode);
    if (NULL == logger->fp)
    {
        fprintf(stderr, "Open log file \"%s\" in %s failure: %s\n", logger->file, filemode, strerror(errno));
        return -2;
    }

    act.sa_handler = log_signal_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGHUP, &act, NULL);

  OUT:
    logger_banner("Initialize");

    return 0;
}

void logger_close(void)
{
    if (!logger || !logger->fp )
        return;

    logger_banner("\nTerminate");
    logger_raw("\n\n\n\n");

    fflush(logger->fp);

    fclose(logger->fp);
    logger->fp = NULL;

    return ;
}

int logger_reopen(void)
{
    int rc = 0;
    char *filemode;

    if( !logger )
        return -1;

    log_rollback_size = logger->size <= 0 ? LOG_ROLLBACK_NONE : logger->size*1024;    /* Unit KiB */

    if (logger->flag & FLAG_LOGGER_CONSOLE )
    {
        fflush(logger->fp);
        logger->fp = stderr;
        return 0;
    }

    if (logger->fp)
    {
        logger_close();
        filemode = log_rollback_size==LOG_ROLLBACK_NONE ? "a+" : "w+";
        logger->fp = fopen(logger->file, filemode); 
        
        if (logger->fp == NULL)
            rc = -2;
    }
    else
    {
        rc = -3;
    }

    if (!rc)
    {
        logger_banner("\nReopen");
    }
    return rc;
}

void logger_term(void)
{
    if(!logger)
        return ;

    logger_close();

    logger = NULL;
}

void logger_raw(const char *fmt, ...)
{
    va_list argp;

    if (!logger || !logger->fp)
        return;

    check_and_rollback();

    va_start(argp, fmt);
    vfprintf(logger->fp, fmt, argp);
    va_end(argp);
}

static void cp_printout(char *level, char *fmt, va_list argp)
{
    char buf[MAX_LOG_MESSAGE_LEN];
    struct tm *local;
    struct timeval now;
    char timestr[256];

    if(!logger)
        return ;

    check_and_rollback();

    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);

    strftime(timestr, 256, LOG_TIME_FMT, local);
    vsnprintf(buf, MAX_LOG_MESSAGE_LEN, fmt, argp);

#ifdef DUMPLICATE_OUTPUT
    printf("%s.%03ld [%s] : %s",
           timestr, now.tv_usec / PRECISE_TIME_FACTOR, level, buf);
#endif

    if (logger->fp)
        fprintf(logger->fp, "%s.%03ld [%s] : %s", timestr, now.tv_usec / PRECISE_TIME_FACTOR, level, buf);

    if (logger->fp)
        fflush(logger->fp);
}

static void cp_printout_line(char *level, char *fmt, char *file, int line, va_list argp)
{
    char buf[MAX_LOG_MESSAGE_LEN];
    struct tm *local;
    struct timeval now;
    char timestr[256];

    if(!logger)
        return ;

    check_and_rollback();

    gettimeofday(&now, NULL);
    local = localtime(&now.tv_sec);

    strftime(timestr, 256, LOG_TIME_FMT, local);
    vsnprintf(buf, MAX_LOG_MESSAGE_LEN, fmt, argp);

#ifdef DUMPLICATE_OUTPUT
    printf("[%s.%03ld] <%s> <%s:%04d> : %s",
           timestr, now.tv_usec / PRECISE_TIME_FACTOR, level, file, line, buf);
#endif

    if (logger->fp)
    {
        fprintf(logger->fp, "[%s.%03ld] <%s> <%s:%04d> : %s",
                timestr, now.tv_usec / PRECISE_TIME_FACTOR, level, file, line, buf);

        fflush(logger->fp);
    }
}

void logger_str(int level, const char *msg)
{
    if (!logger || level>logger->level)
        return;

    check_and_rollback();

    if (logger->fp)
        fwrite(msg, 1, strlen(msg), logger->fp);

    if(logger->fp)
        fflush(logger->fp);
}

void logger_min(int level, char *fmt, ...)
{
    va_list argp;

    if (!logger || level>logger->level)
        return;

    va_start(argp, fmt);
    cp_printout(log_str[level], fmt, argp);
    va_end(argp);
}

void logger_line(int level, char *file, int line, char *fmt, ...)
{
    va_list argp;

    if (!logger || level>logger->level)
        return;

    va_start(argp, fmt);
    cp_printout_line(log_str[level], fmt, file, line, argp);

    va_end(argp);
}

#define LINELEN 81
#define CHARS_PER_LINE 16
static char *print_char =
    "                "
    "                "
    " !\"#$%&'()*+,-./"
    "0123456789:;<=>?"
    "@ABCDEFGHIJKLMNO"
    "PQRSTUVWXYZ[\\]^_"
    "`abcdefghijklmno"
    "pqrstuvwxyz{|}~ "
    "                "
    "                "
    " ???????????????"
    "????????????????" 
    "????????????????" 
    "????????????????" 
    "????????????????" 
    "????????????????";

void logger_dump(int level, char *buf, int len)
{
    int rc;
    int idx;
    char prn[LINELEN];
    char lit[CHARS_PER_LINE + 2];
    char hc[4];
    short line_done = 1;

    if (!logger || level>logger->level)
        return;

    rc = len;
    idx = 0;
    lit[CHARS_PER_LINE] = '\0';

    while (rc > 0)
    {
        if (line_done)
            snprintf(prn, LINELEN, "%08X: ", idx);

        do
        {
            unsigned char c = buf[idx];
            snprintf(hc, 4, "%02X ", c);
            strncat(prn, hc, LINELEN);

            lit[idx % CHARS_PER_LINE] = print_char[c];
        }
        while (--rc > 0 && (++idx % CHARS_PER_LINE != 0));

        line_done = (idx % CHARS_PER_LINE) == 0;
        if (line_done)
        {
#ifdef DUMPLICATE_OUTPUT
            printf("%s  %s\n", prn, lit);
#endif
            if (logger->fp)
                fprintf(logger->fp, "%s  %s\n", prn, lit);
        }
    }

    if (!line_done)
    {
        int ldx = idx % CHARS_PER_LINE;
        lit[ldx++] = print_char[(int)buf[idx]];
        lit[ldx] = '\0';

        while ((++idx % CHARS_PER_LINE) != 0)
  //          strncat(prn, "   ", LINELEN);

#ifdef DUMPLICATE_OUTPUT
        printf("%s  %s\n", prn, lit);
#endif
        if (logger->fp)
            fprintf(logger->fp, "%s  %s\n", prn, lit);

    }
}
