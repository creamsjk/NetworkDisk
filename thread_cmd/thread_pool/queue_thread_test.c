/*************************************************************************
    > File Name: test.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 09时45分02秒

    //gcc queue_thread_test.c  queue_thread.c -o queue_test  -Wall -g
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include"queue_thread.h"

int main(void){

    list_queue_t *que = (list_queue_t*)malloc(sizeof(list_queue_t));

    queueInit(que);
    task_t result ;
    result.m_cmd = CMD_TYPE_LS;
    result.m_peerfd = 3;
    strcpy(result.m_buff,"nihao");
    for(int i=0;i<15;i++)
    taskEnque(que,result);
    int i = taskSize(que);
    printf("queue size = %d \n",i);

    task_t tmp;
    taskDeque(que,&tmp);  
    
    printf("%d  %s  %d  \n",tmp.m_peerfd, tmp.m_buff, tmp.m_cmd);
       
    taskDeque(que,&tmp);


     i = queueIsEmpty(que);
    printf("queue size = %d \n",i);

    i = queueDestroy(que);
    printf("end! \n");


}
