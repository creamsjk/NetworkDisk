/*************************************************************************
    > File Name: test_timer.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月10日 星期六 11时03分20秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include"hashtable.h"
#include"timer.h"

#define MAXTIME 10
int main(void){



     my_timer  timer[MAXTIME];
         for(int i=0; i < MAXTIME ;i++){
  
           timer[i].solt = (my_time_t*)malloc(sizeof(my_time_t));
           timer[i].solt->fd = -1;
           timer[i].solt->pNext = NULL;
         }

           int time_point = 0;


           add_timer(1,timer[1].solt);
           
           add_timer(2,timer[1].solt);
           add_timer(3,timer[1].solt);
           add_timer(4,timer[1].solt);
           add_timer(5,timer[1].solt);

           show_timer(timer[1].solt);

           delete_timer(2, timer[1].solt);
           delete_timer(1, timer[1].solt);
           delete_timer(5, timer[1].solt);

           show_timer(timer[1].solt);

            time_point = 1;
           while(timer[1].solt->pNext != NULL){
                    printf("目前删除的槽是%d \n",time_point);
                 int    close_time = take_timer(timer[time_point].solt);
  
  
                    printf("结束一个fd  %d \n", close_time);
                    break;
                }


           return 0;


}

