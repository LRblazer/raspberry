/********************************************************************************
 *      Copyright:  (C) 2020 LingYun IoT System Studio
 *                  All rights reserved.
 *
 *       Filename:  atcmd.h
 *    Description:  This head file is AT command API for GPRS/NB-IoT/WiFi modules.
 *
 *        Version:  1.0.0(2020年07月26日)
 *         Author:  Guo Wenxue <guowenxue@gmail.com>
 *      ChangeLog:  1, Release initial version on "2020年07月26日 15时24分07秒"
 *                 
 ********************************************************************************/

#ifndef  _ATCMD_H_
#define  _ATCMD_H_

#include "comport.h"

/* AT command common reply message max length  */
#define ATCMD_REPLY_LEN          256

/* AT command reply message got expect or error string */
#define AT_EXPSTR                "OK\r\n"     /* expect string always be OK */
#define AT_ERRSTR                "ERROR\r\n"  /* error string always be ERROR */

/* send_atcmd)( return value status */
enum
{
    ATRES_ERROR,    /* AT command reply got error string, such as "ERROR\r\n" */
    ATRES_EXPECT,   /* AT command reply got expect string, such as "OK\r\n" */
    ATRES_TIMEOUT,  /* AT command not get error/expect string, receive util timeout */
};

/*  Description: this function used to send an AT command through a serial port and receive reply
 *               message from the corresponding module, and it will return once get expet/error 
 *               string or timeout.
 *
 *    Arugments: 
 *         $comport: the serial port which connected to GPRS/3G/4G/NB-IoT/WiFi/BLE/Zigbee/LoRa...
 *           $atcmd: the AT command need to be sent
 *         $timeout: wait for module reply for AT command timeout value, unit micro seconds(ms)
 *          $exepct: the expect string reply from hardware module;
 *           $error: the error string reply from hardware module;
 *            $rbuf: the module reply message output buffer, 
 *            $size: the output buffer ($reply) size
 *
 * Return value: <0: Function error   0: Got error string   1: Got expect string 2: Receive util timeout
 *
 */
int send_atcmd(comport_t *comport, char *atcmd, unsigned long timeout, char *expect, char *error, char *rbuf, int size);


/*
 *  Description: Send AT command which will only reply by "OK" or "ERROR", such as AT:
 *                  Reply:   \r\nOK\r\n
 *
 * Return Value:  0: OK     -X: ERROR
 */
int send_atcmd_check_ok(comport_t *comport, char *atcmd, unsigned long timeout);


/*
 *  Description: Send AT command which will reply by a value directly in a single line, such as AT+CGMM:
 *                  Reply:   \r\nEC20F\r\nOK\r\n
 *
 * Return Value:  0: OK     -X: ERROR
 */
int send_atcmd_check_value(comport_t *comport, char *atcmd, unsigned long timeout, char *rbuf, int size);


/*
 *  Description: Send AT command which will reply by the value  with a prefix "+CMD: " line, such as AT+CSQ:
 *                  Reply:   \r\n+CSQ: 26,99\r\nOK\r\n  
 *
 * Return Value:  0: OK     -X: ERROR
 */
int send_atcmd_check_request(comport_t *comport, char *atcmd, unsigned long timeout, char *rbuf, int size);


#endif   /* ----- #ifndef _ATCMD_H_  ----- */

