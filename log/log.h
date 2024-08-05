/*************************************************************************
    > File Name: log.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 17时16分38秒
 ************************************************************************/

#ifndef  __LOG_H
#define  __LOG_H

#include"../type.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<sys/time.h>

typedef struct  log_s{

    char dir_name[128]; //路径名
    char log_name[128]; //log文件名
    int m_split_lines;  //日志最大行数
    int m_log_buf_size; //日志缓冲区大小
    long long m_count;  //日志行数记录
    int m_today;        //因为按天分类,记录当前时间是那一天
    FILE *m_fp;         //打开log的文件指针
    char *m_buf;

}log_t;


int log_init(log_t*  log){

    char * file_name = "ServerLog";
    log->m_log_buf_size = 8192;
    log->m_split_lines = 5000000;

    log->m_buf = (char *)malloc(sizeof(char) * log->m_log_buf_size);
    memset(log->m_buf, '\0',log->m_log_buf_size);
    
    time_t t = time(NULL);
     struct tm *sys_tm = localtime(&t);
     struct tm my_tm = *sys_tm;

     const char*p = strrchr(file_name, '/');
     char log_full_name[256]={0};

     if( p == NULL){

         snprintf(log_full_name, 255, "%d_%02d_%02d_%s", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, file_name);
         
     }else{

         strcpy(log->log_name, p + 1);
         strncpy(log->dir_name, file_name, p - file_name + 1);
         snprintf(log_full_name, 255 , "%s%d_%02d_%02d_%s", log->dir_name, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, log->log_name);

     }
     log->m_today = my_tm.tm_mday;

     log->m_fp = fopen(log_full_name, "a");
     if(log->m_fp == NULL){

         return 1;
     }

     return 0;

}


void write_log(log_t *log, char * message){
     struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    char s[16] = {0};

     strcpy(s, "[info]:");

     log->m_count++;

     if(log->m_today != my_tm.tm_mday  || log->m_count % log->m_split_lines == 0){

         char new_log[256] = {0};
         fflush(log->m_fp);
         fclose(log->m_fp);
         char tail[16]={0};

         snprintf(tail, 16, "%d_%02d_%02d_", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);

         if(log->m_today != my_tm.tm_mday){

             snprintf(new_log, 255, "%s%s%s", log->dir_name, tail, log->log_name);
             log->m_today = my_tm.tm_mday;
             log->m_count = 0;
         }else{

             snprintf(new_log, 255, "%s%s%s.%lld", log->dir_name, tail, log->log_name, log->m_count / log->m_split_lines);
         }
         log->m_fp = fopen(new_log, "a");
     }
     //写入具体时间内容格式
      int n = snprintf(log->m_buf, 48, "%d-%02d-%02d %02d:%02d:%02d.%06ld %s ",
                     my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                     my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec, s);
    
    //int m = snprintf(log->m_buf + n,log-> m_log_buf_size - n - 1, message);
    //log->m_buf[n + m] = '\n';
    //log->m_buf[n + m + 1] = '\0';
    //
    strcat(log->m_buf,message);

    log->m_buf[n + strlen(message)] = '\n';
    log->m_buf[n + strlen(message) + 1] = '\0';
    
   // printf("========= %s ",log->m_buf);
    fputs(log->m_buf, log->m_fp);

     fflush(log->m_fp);



}




#endif
