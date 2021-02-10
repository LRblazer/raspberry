/*********************************************************************************
 *      Copyright:  (C) 2021 big world
 *                  All rights reserved.
 *
 *       Filename:  clock.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(27/01/21)
 *         Author:  xuxinhua <1411032148@qq.com>
 *      ChangeLog:  1, Release initial version on "27/01/21 14:10:22"
 *                 
 ********************************************************************************/
#include <ctype.h>
#include <pthread.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <errno.h>
#include <time.h>
#include <signal.h>


void print_usage(char *progname);
int judge_time(char* c_time)
{
    int t = 0;
    char min_hour[10];
    char  *p[3] ;
    int   i=0;
    memset(min_hour,0, sizeof(min_hour));

    for (t; t < strlen(c_time); ++t)
    {
        min_hour[t]= c_time[t];
    }
    
    char  *temp = strtok(min_hour,":");
    
    while( temp )
    {
    //    printf("%s\n",temp);
        p[i]=temp;
   //     printf("\np[%d]:%s\n",i, p[i]);
        temp = strtok(NULL,":");
        i++;
    }

    int hour =atoi(p[0]);
    int minite =atoi(*(p+1));
    int second = atoi(*(p+2));
    
  //  printf("%d----%d----%d\n",hour, minite, second);

    time_t  timep;
    struct  tm *lp;
    time(&timep);
    lp = localtime(&timep);

    if(((hour- (lp->tm_hour))==0)&&((minite- ( lp->tm_min))==0))
    {
    //    printf("\nit is time to get up!!!\n");
        return 1;
    }
    else
    {
   //     printf("\nit is not clock time!!!\n");
        return 0;
    }
}


char *judge_music(char *music_buf_cmd)
{
     char    *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
     time_t  timep;
     struct  tm *p;
     time(&timep);
     p = localtime(&timep); /* 取得当地时间*/
                         
  //   printf("today is:%d\n",1+ p->tm_wday);

     switch(p->tm_wday)
     {
        case 0:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday+7 );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        case 1:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        case 2:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        case 3:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        case 4:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        case 5:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        case 6:
            sprintf(music_buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/%d.mp3", p->tm_wday );
            printf( "music_buf_cmd:%s\n", music_buf_cmd);
            break;
        default:
            break;
      
     }
     return music_buf_cmd;

}
  //  judge_time( clock_time);

void judge_intclock()
{
     time_t  timep;
     struct  tm *p;
     time(&timep);
     p = localtime(&timep); /* 取得当地时间*/
                         
    char buf_cmd[50] = {};
    memset(buf_cmd,0,sizeof(buf_cmd));
  //   printf("today is:%d\n",1+ p->tm_wday);

     if((p->tm_min==0)&&(p->tm_sec==0))
     {
     switch(p->tm_hour)
     {
        case 9:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 10:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 11:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 12:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 13:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 14:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 15:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 16:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 17:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 18:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 19:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 20:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        case 21:
            sprintf(buf_cmd, "/usr/bin/mplayer /home/pi/biye/Music/intclock/%d.mp3", p->tm_hour);
            printf( "buf_cmd:%s\n", buf_cmd);
            system(buf_cmd);
            break;
        default:
            break;
     }
    }
    return;
}
// execl("/usr/bin/amixer","amixer","set", "Headphone", "80%",NULL);
//  system("/usr/bin/amixer set Headphone 90%");
//  execl("/usr/bin/mplayer","mplayer","/home/pi/Music/liu.mp3",(char * )0);


int     g_sigstop = 0;
void signal_stop(int signum)
{
    {
        printf("SIGTERM signal kill this proccess\n");
        g_sigstop = 1;
    }
}
                


int main(int argc, char **argv)
{


    char *clock_time =NULL;
    int daemon_run = 0;
    char *progname = NULL;
    int opt;

    char music_buf_cmd[50] = {};
    memset(music_buf_cmd,0,sizeof(music_buf_cmd));

    signal(SIGINT, signal_stop);

    struct  option  long_options[] =
    {
        {"daemon",no_argument, NULL, 'b'},
        {"time", required_argument, NULL, 't'},
        {"help", no_argument, NULL, 'h' },
        {NULL, 0, NULL, 0}
    };
    
    progname = basename(argv[0]);
    
    while ((opt = getopt_long(argc, argv, "bt:h", long_options, NULL)) != -1) 
    {
        switch (opt) 
        {
            case 'b':
                daemon_run = 1;
                break;
            case 'h':
                print_usage(progname);
                return EXIT_SUCCESS;
            case 't':
                clock_time =optarg;
                printf("clock time: %s\n", clock_time);
                break;   
            default:
                break;    
        }   
    }

    if( daemon_run ) 
    {
        int my_txt_log = -1;
        printf("Program %s is running at the background now\n", argv[0]);
        if ((daemon(1, 1)) < 0)
        {
            printf("Deamon failure : %s\n", strerror(errno));
            return 0;
        }
                        
        //创建日志系统，程序后台运行后，将所有打印信息打印在日志文件中
        my_txt_log = open("/home/pi/biye/Music/mytext.log", O_RDWR|O_CREAT|O_TRUNC, 0666);
        if (my_txt_log < 0)
        {
            printf("open the log failure : %s\n", strerror(errno));
            return 0;
        }
        //标准输出及标准出错重定向，重定向至日志文件
        dup2(my_txt_log, STDOUT_FILENO);
        dup2(my_txt_log, STDERR_FILENO);
    }

  //  judge_music(music_buf_cmd);
#if 1
    pid_t pid;
    printf("Parent process PID[%d] start running...\n", getpid() );

    pid = fork();

    if(pid < 0)
    {
        printf("fork() create child process failure: %s\n", strerror(errno));
        return -1;
    }
    else if( pid == 0)
    {
        while(!g_sigstop)
        {
            while(judge_time(clock_time)==1)
            { 
                char sdcmd[50]={};
                int volume= 50;
                for(volume=50;volume <= 100;volume+=1 )
                {
                    sprintf(sdcmd, "/usr/bin/amixer set Headphone '%d'%%", volume );
                    printf("volume=============================%d \n",volume);
       
                    system(sdcmd);

                    sleep(5);
                    if(g_sigstop==1)
                    {
                        break;
                    }
            
                }
            }
        }
      
            printf("son exit:%d\n",g_sigstop);
            return 0;
    }
    else if(pid >0)
    {   
        while(!g_sigstop)
        {
            while(judge_time(clock_time)==1)
            {
                printf("-----------------------------------------------------------------\n");
                judge_music(music_buf_cmd);
                system(music_buf_cmd);
                system("/usr/bin/mplayer /home/pi/biye/Music/dange.mp3");
                printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
           // return(0);
            }
        }
        printf("father exit:%d\n",g_sigstop);
        return 0;
    }
#endif 
    printf("over\n");
    return 0;
}


void print_usage(char *progname)
{
    printf("%s usage: \n", progname);      
    printf("-t(--time): clock time \n");
    printf("-b(--daemon): put to back\n");
    printf("-h(--Help): print this help information.\n");
    printf("for example:  ./a.out -t 7:00 \n");
    return ;
}







