/********************************************************************************
 *      Copyright:  (C) 2020 big world
 *                  All rights reserved.
 *
 *       Filename:  esp_mqtt.h
 *    Description:  This head file 
 *
 *        Version:  1.0.0(13/08/20)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "13/08/20 08:42:16"
 *                 
 ********************************************************************************/

#ifndef _ESP_MQTT_H_
#define _ESP_MQTT_H_

#include "comport.h"
#include "logger.h"
#include "atcmd.h"
#include "util_time.h"
#include "delay.h"

int check_esp(comport_t *comport);
int join_route(comport_t *comport);
int join_mqtt(comport_t *comport);
int mqtt_sub(comport_t *comport);
int mqtt_pub(comport_t *comport, float *temp, int state, int lightness);
int disconn_mqtt(comport_t *comport);


#endif 
