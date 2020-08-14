/*********************************************************************************
 *      Copyright:  (C) 2020 LingYun IoT System Studio
 *                  All rights reserved.
 *
 *       Filename:  atcmd.c
 *    Description:  This file is AT command API for GPRS/NB-IoT/WiFi modules.
 *                 
 *        Version:  1.0.0(2020年07月26日)
 *         Author:  Guo Wenxue <guowenxue@gmail.com>
 *      ChangeLog:  1, Release initial version on "2020年07月26日 15时39分53秒"
 *                 
 ********************************************************************************/

#include <ctype.h>
#include "atcmd.h"
#include "logger.h"
#include "util_time.h"



/*  Description: this function used to send an AT command through a serial port and receive reply
 *               message from the corresponding module, it will return once get expet/error 
 *               string or timeout.
 *
 *    Arugments: 
 *         comport: the serial port which connected to GPRS/3G/4G/NB-IoT/WiFi/BLE/Zigbee/LoRa...
 *           atcmd: the AT command need to be sent
 *         timeout: wait for module reply for AT command timeout value, unit micro seconds(ms)
 *          exepct: the expect string reply from hardware module;
 *           error: the error string reply from hardware module;
 *           reply: the module reply message output buffer, 
 *            size: reply message output buf size
 *
 * Return value: <0: Function error   0: Got error string   1: Got expect string 2: Receive util timeout
 */

int send_atcmd(comport_t *comport, char *atcmd, unsigned long timeout, char *expect, char *error, char *reply, int size)
{
    int                    rv = 0;
    int                    res = -4;
    int                    len = 0;
    int                    bytes = 0;
    char                   at[256];
    unsigned long          expire;
    char                  *ptr;
    int                    i=0;
    char                   buf[1024];

    if( !comport || !atcmd )
    {
        log_err("Invalid input arguments\n");
        return -1;
    }

    if( comport->fd <= 0 )
    {
        log_err("comport[%s] not opened\n");
        return -2;
    }

    /* flushes both data received but not read, and data written but not transmitted in serial port */
    tcflush(comport->fd, TCIOFLUSH);

    ptr = atcmd;
    while(*ptr != '\r' && *ptr!='\0' )
    {
        at[i++] = *ptr++;
    }
    at[i] = 0; 

    rv=comport_send( comport, atcmd, strlen(atcmd) );
    if(rv < 0)
    {
        log_err("send AT command \'%s\' to \'%s\'", at, comport->dev_name);
        return -3;
    }
    log_info("Send AT command '%s' ok\n", at, rv);

    memset( buf, 0, sizeof(buf) );
    expire = time_now() + timeout;

    res = ATRES_TIMEOUT;
    while( time_before(time_now(), expire) )
    {
        rv = comport_recv(comport, &buf[bytes], sizeof(buf)-bytes, 10);
        if( rv < 0 ) 
        {
            log_err("receive AT command '%s' reply failure, rv=%d\n", at, rv);
            rv = -4;
            break;
        }

        bytes += rv;

        if( expect && strstr(buf, expect) ) 
        {
            res = ATRES_EXPECT;
            log_info("AT command '%s' got exepct reply string\n", at);
            break;
        }
        else if( error && strstr(buf, error) ) 
        {
            res = ATRES_ERROR;
            log_info("AT command '%s' got error reply string\n", at);
            break;
        }
    }

    if( reply && size>0 )
    {
        memset(reply, 0, size);

        log_info("AT command copy out reply message\n");
        len = bytes>size ? size : bytes;
        strncpy(reply, buf, len);
    }

    return res;
}


/*
 *  Description: Send AT command which will only reply by "OK" or "ERROR", such as AT:
 *                 Reply:   \r\nOK\r\n
 * Return Value: 0: OK     -X: Failure
 */
int send_atcmd_check_ok(comport_t *comport, char *atcmd, unsigned long timeout)
{
    int                     rv;

    if( !comport || !atcmd )
    {
        log_err("Invalid input arguments\n");
        return -1;
    }

    rv=send_atcmd(comport, atcmd, timeout, AT_EXPSTR, AT_ERRSTR, NULL, 0); 
    if( ATRES_EXPECT == rv )
    {
        return 0;
    }
    else 
    {
        log_err("Send AT command got ERROR reply or failure, rv=%d\n", rv);
        return -1;
    }
}


/*
 *  Description: Send AT command which will reply by a value directly in a single line, such as AT+CGMM:
 *                  Reply:   \r\nEC20F\r\nOK\r\n
 *
 * Return Value: 0: OK     -X: Failure
 */
int send_atcmd_check_value(comport_t *comport, char *atcmd, unsigned long timeout, char *reply, int size)
{
    int                     i = 0;
    int                     rv;
    char                    buf[ATCMD_REPLY_LEN];
    char                   *ptr;

    if( !comport || !atcmd || !reply || size<=0 )
    {
        log_err("Invalid input arguments\n");
        return -1;
    }

    rv = send_atcmd(comport, atcmd, timeout, AT_EXPSTR, AT_ERRSTR, buf, ATCMD_REPLY_LEN);
    if( rv <= 0 )
    {
        log_err("Send AT command got ERROR reply or failure, rv=%d\n", rv);
        return -2;
    }

    ptr=strchr(buf, '\n'); /* found '\n' before the value */
    if( !ptr )
    {
        log_err("reply message got wrong\n");
        return -3;
    }

    ptr++;   /* skip '\n'  */

    memset(reply, 0, size);
    while(*ptr!='\r' && i<size-1)
    {
        reply[i++] = *ptr;
        ptr ++;
    }

    return 0;
}


/*
 *  Description: Send AT command which will reply by the value  with a prefix "+CMD: " line, such as AT+CSQ:
 *                  Reply:   \r\n+CSQ: 26,99\r\nOK\r\n 
 *
 * Return Value: 0: OK     -X: Failure
 */
int send_atcmd_check_request(comport_t *comport, char *atcmd, unsigned long timeout, char *reply, int size)
{
    int                     i = 0;
    int                     rv;
    char                    buf[ATCMD_REPLY_LEN];
    char                   *ptr;

    if( !comport || !atcmd || !reply || size<=0 )
    {
        log_err("Invalid input arguments\n");
        return -1;
    }

    rv = send_atcmd(comport, atcmd, timeout, AT_EXPSTR, AT_ERRSTR, buf, ATCMD_REPLY_LEN);
    if( rv <= 0 )
    {
        log_err("Send AT command got ERROR reply or failure, rv=%d\n", rv);
        return -2;
    }

    ptr=strchr(buf, '+');  /* found '+' before the value */
    if( !ptr )
    {
        log_err("reply message got wrong\n");
        return -3;
    }
    ptr++;   /* skip '+'  */


    ptr=strchr(buf, ':');  /* found '+' before the value */
    if( !ptr )
    {
        log_err("reply message got wrong\n");
        return -3;
    }
    ptr++;   /* skip ':'  */


    memset(reply, 0, size);
    while(*ptr!='\r' && i<size-1)
    {
        if( !isspace(*ptr) ) /* skip space,\r,\n ...  */
            reply[i++] = *ptr;
        ptr ++;
    }

    return 0;
}

