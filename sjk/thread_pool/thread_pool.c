/*************************************************************************
    > File Name: thread_pool.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 10时06分36秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include"thread_pool.h"
#include<assert.h>
#include<pthread.h>

void process(){

    printf("%ld执行任务  over\n",pthread_self());

}


void *work(void* arg){
    // printf("work .......\n");
    threadpool_t *pool = (threadpool_t* )arg;

    while(1){
       task_t  tmp;
       int ret = taskDeque(pool->m_que,&tmp);
       //   printf("ddpeerfd  %d  \n",tmp.m_peerfd);
       assert(ret == 0);
        int peerfd = tmp.m_peerfd;
       if(peerfd > 0){
           process();
       }else{
           break;
       }

    }
    printf("sub thread %ld is exiting. \n",pthread_self());
    return NULL;


}


int threadpoolInit(threadpool_t *pool,int num){
 
    assert(pool);
    pool->m_pthreads = (pthread_t*)malloc(sizeof(pthread_t)*num);
    assert(pool->m_pthreads);

    pool->m_pthreadNum = num;
    assert(pool->m_pthreadNum  > 0);
    pool->m_que = (list_queue_t*)malloc(sizeof(list_queue_t));
    queueInit(pool->m_que);    

    // printf("threadpoolInit  success \n");

    return 0;
}

int threadpoolDestory(threadpool_t *pool){

    // printf("threadpoolDestory  begin  \n");

    int ret = -1;
    //等待子线程结束
    for(int i=0;i<pool->m_pthreadNum;i++){
         ret = pthread_join(pool->m_pthreads[i],NULL);
        assert(ret == 0);

    }
    assert(pool);
    assert(pool->m_pthreads);
    broadcastAll(pool->m_que);

    free(pool->m_pthreads);
    pool->m_pthreads = NULL;
    pool->m_pthreadNum = 0;
    queueDestroy(pool->m_que);

    return 0;
}


int threadpoolStart(threadpool_t *pool){
    //printf("pthreaSTARt begin \n");
    int ret  = 0;
    int num = pool->m_pthreadNum;
    for(int i=0;i<num;i++){
        ret =  pthread_create(&pool->m_pthreads[i],NULL,work,pool);
        assert(ret == 0);
    }


    // printf("threadpoolStart success \n");
    return 0;

}


int threadpoolStop(threadpool_t *pool){
    //  printf("threadpoolStop   begin   \n");

     if(pool == NULL){
        return 0;
    }
     //   printf("pool num  %d \n",pool->m_pthreadNum);
     for(int i=0;i<pool->m_pthreadNum;i++){
               task_t result ;
               result.m_cmd = CMD_TYPE_LS;
               result.m_peerfd = -1;
               strcpy(result.m_buff,"nihao");
               taskEnque(pool->m_que,result);
   
           }


     //      printf("STOP success \n");
    return 0;

}

