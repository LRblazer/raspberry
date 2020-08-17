/* ********************************************************************************
 *        Copyright:  (C) 2020 big world
 *                     All rights reserved.
 *    
 *         Filename:  main.c
 *      Description:  This file 
 *                        
 *          Version:  1.0.0(14/08/20)
 *           Author:  xuxinhua <1411032148@qq.com>
 *        ChangeLog:  1, Release initial version on "14/08/20 15:17:40"
 *                 
 **********************************************************************************/
#include <pthread.h>
#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "atcmd.h"
#include "logger.h"
#include <bcm2835.h>
#include "oled.h"
#include "menu_show.h"
#include "delay.h"
#include "comport.h"
#include "logger.h"
#include "atcmd.h"
#include "temp.h"
#include "get_time.h"
#include "esp_mqtt.h"
#include "gpio.h"
#include "sqlite3.h"
#include "my_sqlite3.h"

#if 0

#define R_PIN 13
#define Y_PIN 19
#define G_PIN 26

#define turn_light_on(x) bcm2835_gpio_write(x, HIGH) 
#define turn_light_off(x) bcm2835_gpio_write(x, LOW) 

void GPIO_init()
{
    //设置引脚为输出状态，即初始化GPIO
    bcm2835_gpio_fsel(R_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(Y_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(G_PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("gpio init competely\n");
}

#endif

int g_stop = 0;
void sig_exit(int sig_num)
{
    printf("catch Chrl+C\n");
    if(sig_num == SIGINT)
        g_stop = 1;
}

#if 0
//80s上传一次数据到腾讯云，防止发送信息过多
comport_t        *comport = NULL;
float             temp = 0;
uint8_t           light_state = 2;
void sig_alarm(int sig_num)
{
    alarm(80);
    printf("catch alarm +++++++++++++++++++++\n");
    printf("start pub +++++++++++++++++++++\n");

    if( mqtt_pub(comport,&temp, light_state) < 0 )
    {
        printf("pub unsuccessfully\n");
        disconn_mqtt(comport);
        exit;
    }

    printf("pub down\n");
    delay_ms(1000);
}

#endif

typedef void *(THREAD_BODY) (void *thread_arg);
void *thread_worker(void *ctx);
int thread_start(pthread_t * thread_id, THREAD_BODY * thread_workbody, void *thread_arg);  

//定义锁
typedef struct   worker_ctx_s 
{ 
        pthread_mutex_t   lock; 
} worker_ctx_t; 

//声明为全局变量
comport_t         *comport;

int main (int argc, char *argv[])
{ 
    logger_t          logger;

    //下面三个数组用于存放将要存入数据的采集到的数据
    char              iot_dev[5] = "dev1";
    char              sqlite_temp[10];
    char              sqlite_time[20];

//    comport_t        *comport = NULL;
    char             *dev_name = "/dev/ttyUSB0";
    char             *settings = "8N1N";
    unsigned long     baudrate = 115200;

    char              buf[256];

    float             temp = 0;
    int               temp_rv = 0;

    uint8_t           light_state = 2;

    int               mqtt_state = -1;

    pthread_t         tid;
    worker_ctx_t      worker_ctx;

    sqlite3 * db;
    char *errmsg;

    //打开数据库
    if(sqlite3_open("./../../temp.db", &db) != SQLITE_OK)
    {
        printf("%s\n",sqlite3_errmsg(db));
        return -1;
    }
    else
    {
        printf("open temp.db success\n");
    }

    signal(SIGINT,sig_exit);
    //     signal(SIGALRM,sig_alarm);
    //     alarm(80);

    //gpio init
    if (!bcm2835_init())
        return 1;

    //初始化OLED
    OLED_Init();                 //initialise OLED module  
    OLED_Clear(0);               //clear OLED screen(black)

    GPIO_init();


    //测试黄灯（台灯）的可工作性
    turn_light_on(Y_PIN);

    //日志初始化
    if ( logger_init(&logger, DBG_LOG_FILE, LOG_LEVEL_NRML, LOG_ROLLBACK_NONE) || logger_open() )
    {
        printf("initialise logger system failure\n");
        return -1;
    }
    log_nrml("logger system start ok\n");

    //串口初始化并打开串口
    if( !(comport=comport_init(dev_name, baudrate, settings)) || comport_open(comport)<0 )
    {
        log_err("initialise comport[%s] for AT command test failure: %s\n", dev_name);
        return -2;
    }
    log_nrml("open comport[%s] for AT command ok\n", comport->dev_name);

    //检测esp8266是否可以正常工作
    if(  check_esp(comport ) < 0 )
    {
        printf("check_esp unsuccessfully\n");
        return -3;
    }

    //esp8266 联网
    if(  join_route(comport) < 0 )
    {
        printf("join route unsuccessfully\n");
        return -4;
    }

    //esp8266 连接腾讯云MQTT服务器
    mqtt_state = join_mqtt(comport);
    if( mqtt_state < 0 ) 
    {
        printf("join mqtt unsuccessfully\n");
        disconn_mqtt(comport);
        return -5;
    }

    //在OLED上显示是否连接成功
    show_connect_result(mqtt_state);
    //连接成功亮绿灯
    //bcm2835_gpio_write(G_PIN, HIGH);
    turn_light_on(G_PIN);

    //订阅主题
    if( mqtt_sub(comport) < 0 ) 
    {
        printf("sub unsuccessfully\n");
        disconn_mqtt(comport);
        return -6;
    }

    //创建子线程
    thread_start(&tid, thread_worker, &worker_ctx);

    while(!g_stop)
    {
        sleep(3);
        //获取黄灯状态和温度
        light_state = bcm2835_gpio_lev(Y_PIN);
        temp_rv  = get_temperature(&temp);

#if 1
        snprintf(sqlite_temp,5,"%.2f C",temp);
        get_time(sqlite_time);
        do_insert_query(db, iot_dev,sqlite_time,sqlite_temp);

#endif 
        if( temp_rv < 0 ) 
        {
            printf("ERROR:  get temperature failure %d\n", temp_rv);   
            disconn_mqtt(comport);
            return -1;
        }
        printf(" temperature: %f\n", temp);

        //如果温度高于30度，亮起红灯             
        if (temp > 30)
        {
            turn_light_on(R_PIN);
            //bcm2835_gpio_write(R_PIN, HIGH);         
        }
        //将黄灯的状态和温度显示在屏幕上
        show_msg(&temp, light_state);
        delay_ms(8000);


        //将采集的温度和台灯状态上发到腾讯云
#if 1 
        pthread_mutex_trylock(&worker_ctx.lock); 
        if( mqtt_pub(comport,&temp, light_state) < 0 )
        {
            printf("pub unsuccessfully\n");
            disconn_mqtt(comport);
            return -7;
        }
        pthread_mutex_unlock(&worker_ctx.lock); 
#endif 
        delay_ms(1000);
    }



    //释放互斥锁
    pthread_mutex_destroy(&worker_ctx.lock);



    delay_ms(8000);
    //退出程序前，断开与腾讯云的连接
    if( disconn_mqtt(comport) < 0 )
    {
        printf("disconn mqtt unsuccessfully\n");
        disconn_mqtt(comport);
        return -8;
    }


    //关闭所有灯
    bcm2835_gpio_write(R_PIN, LOW);
    bcm2835_gpio_write(Y_PIN, LOW);
    bcm2835_gpio_write(G_PIN, LOW);
    bcm2835_close();

    //关闭串口
    comport_term(comport);
    //关闭日志系统       
    logger_term();
    //关闭数据库
    sqlite3_close(db);

    return 0; 
} 

//创建子线程
int thread_start(pthread_t * thread_id, THREAD_BODY * thread_workbody, void *thread_arg)
{  
    int                 rv = -1;  
    pthread_attr_t      thread_attr;
    if( pthread_attr_init(&thread_attr) )     
    { 
        printf("pthread_attr_init() failure: %s\n", strerror(errno));      
        goto CleanUp;  
    }

    if( pthread_attr_setstacksize(&thread_attr, 120*1024) ) 
    { 
        printf("pthread_attr_setstacksize() failure: %s\n", strerror(errno));     
        goto CleanUp; 
    }

    if( pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED) )       
    {   
        printf("pthread_attr_setdetachstate() failure: %s\n", strerror(errno));  
        goto CleanUp; 
    }

    /*  Create the thread */  
    if( pthread_create(thread_id, &thread_attr, thread_workbody, thread_arg) )    
    {    
        printf("Create thread failure: %s\n", strerror(errno));      
        goto CleanUp;
    }
    rv = 0;            
CleanUp:       
    /*  Destroy the  attributes  of  thread */     
    pthread_attr_destroy(&thread_attr);  
    return rv;
}


//子线程处理函数，用于接受来自腾讯云下发的命令
void *thread_worker(void *ctx)
{
            int                  rv;    
            char                 r_buf[1024];     
            int                  i;
            worker_ctx_t        *ictx = (worker_ctx_t *)ctx;
               
            if( !ctx ) 
            {  
                printf("Invalid input arguments in %s()\n", __FUNCTION__);     
                pthread_exit(NULL); 
            }
             
            
            printf("Child thread start to listen output comport....\n");
            
            while(1)       
            {  

                pthread_mutex_trylock(&ictx->lock); 
                                                                                
                rv = comport_recv(comport,r_buf,1024 , 1);
                if (rv > 0 && strstr(r_buf, "control")!= NULL )
                {   
                    printf("receive[ %d ] buf from tengxun is: %s\n ", rv, r_buf);     
                    memset(r_buf, 0, sizeof(r_buf));        
                    rv = comport_recv(comport,r_buf,1024 , 2);
                    printf("receive[ %d ] buf from tengxun is: %s\n ", rv, r_buf);     
                    char *ptr = strstr(r_buf, "light");
                    ptr += 7 ;
                    
                    char state[2];
                    strncpy(state, ptr, 1);

                    printf("light control state:%c\n", state[0]);

                    if ( state[0] == '1')
                    {
                        printf("turn on yellow light \n");
                        turn_light_on(Y_PIN);  

                    }else if(state[0]== '0')
                    {
                        printf("turn off yellow light \n");
                        turn_light_off(Y_PIN); 
                    }
                }
                pthread_mutex_unlock(&ictx->lock); 
                memset(r_buf, 0, sizeof(r_buf));        
            }
} 
