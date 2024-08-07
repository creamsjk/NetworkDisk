/*************************************************************************
    > File Name: queue_thread.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 09时05分08秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"queue_thread.h"
#include<assert.h>

int queueInit(list_queue_t *que){

    assert(que);
    
    que->m_pFront = (queue_thread_t*)malloc(sizeof(queue_thread_t));
    que->m_pRear = (queue_thread_t*)malloc(sizeof(queue_thread_t));
    assert(que->m_pFront);
    assert(que->m_pRear);

    que->m_pFront->pNext = NULL;
    que->m_pRear->pNext = NULL;
    que->m_queSize = 0;
    int ret = pthread_mutex_init(&que->m_mutex,NULL);
    assert(ret == 0);
    ret = pthread_cond_init(&que->m_cond,NULL);
    assert(ret == 0);
    que->m_flag = 0;
    
    //  printf("Init sucess \n");

    return 0;

}

int queueDestroy(list_queue_t *que){

    assert(que);

    if(!que->m_pFront){
        free(que->m_pFront);
    }

    if(!que->m_pRear){
        free(que->m_pRear);
    }
    que->m_pFront = NULL;
    que->m_pRear = NULL;
    
   int ret =  pthread_mutex_destroy(&que->m_mutex);
   //printf("!!!!!!!!!!!!!  %d \n",ret);
   assert(ret == 0);
   ret = pthread_cond_destroy(&que->m_cond);
   assert(ret == 0);

   que = NULL;
   // printf("destory suucess \n");

   return 0;

}

int queueIsEmpty(list_queue_t *que){
    assert(que);

    return que->m_queSize == 0?0:1;
    
}

int taskSize(list_queue_t *que){
    assert(que);

    return que->m_queSize;
}

int taskEnque(list_queue_t *que, const task_t result){

    //尾插插法

    if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }

    int ret = pthread_mutex_lock(&que->m_mutex);
    assert(ret == 0);

    queue_thread_t *node = (queue_thread_t*)malloc(sizeof(queue_thread_t));
    assert(node);
    node->pNext = NULL;
    node->m_task.m_peerfd = result.m_peerfd;
    node->m_task.m_cmd = result.m_cmd;
    strncpy(node->m_task.m_buff,result.m_buff,sizeof(node->m_task.m_buff));
    strncpy(node->m_task.m_pwd, result.m_pwd, sizeof(result.m_pwd));
    
    node->m_task.m_cmd = result.m_cmd;


    if(que->m_queSize == 0){
        que->m_pFront->pNext = node;
        que->m_pRear->pNext = node;

    }
    else{
     que->m_pRear->pNext->pNext = node;
     que->m_pRear->pNext = node;
    }
    que->m_queSize++;

    ret = pthread_cond_signal(&que->m_cond);
    assert(ret == 0);
    ret = pthread_mutex_unlock(&que->m_mutex);
    assert(ret == 0);

    //    printf("queue insert success \n");
    return 0;


}

int taskDeque(list_queue_t *que, task_t* result){
    //从头部拿出来一个

    //printf("task begin \n");
    if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }
    int ret = pthread_mutex_lock(&que->m_mutex);
    assert(ret == 0);

    while(que->m_queSize == 0){
       pthread_cond_wait(&que->m_cond,&que->m_mutex);
    }
    //一定不为0
    if(que->m_queSize == 0){
        ret = pthread_mutex_unlock(&que->m_mutex);
        assert(ret ==0);
        return 0;
    }

    //将传入的参数进行修改
    queue_thread_t *tmp = que->m_pFront->pNext;
    *result = tmp->m_task;
    //strcpy(result->m_buff,tmp->m_task.m_buff);
    // result->m_cmd = tmp->m_task.m_cmd;
    // result->m_peerfd = tmp->m_task.m_peerfd; 

    queue_thread_t *t = que->m_pFront->pNext;
    que->m_pFront->pNext = t->pNext;
    free(t);
    t = NULL;
    que->m_queSize--;

    ret = pthread_mutex_unlock(&que->m_mutex);
    assert(ret ==0);

    //  printf("queue take success \n");

    return 0;


}

int broadcastAll(list_queue_t *que){

     if(!que){
        printf("que is  NULL \n");
        exit(-1);
    }
    int ret = pthread_cond_broadcast(&que->m_cond);
    assert(ret == 0);
    return 0;

}









