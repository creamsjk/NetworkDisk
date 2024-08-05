/*************************************************************************
    > File Name: queue_thread.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月03日 星期六 17时34分55秒
 ************************************************************************/

#ifndef  QUEUE_THREAD_H
#define  QUEUE_THREAD_h

#include"type.h"
#include<pthread.h>
#include<semaphore.h>

typedef  struct queue_thread_s{

    task_t m_task;
    struct queue_thread_s *pNext;
}queue_thread_t;


typedef struct list_queue_s{

    queue_thread_t *m_pFront;
    queue_thread_t *m_pRear;
    int m_queSize;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    int m_flag;

}list_queue_t;


//初始化阻塞队列
int queueInit(list_queue_t *que);

//销毁阻塞队列
int queueDestroy(list_queue_t *que);

//判断阻塞队列是否为空
int queueIsEmpty(list_queue_t *que);

//返回阻塞队列的size数量
int taskSize(list_queue_t *que);

//向阻塞队列添加元素
int taskEnque(list_queue_t *que, task_t node);

//从阻塞队列取出来一个元素
int taskDeque(list_queue_t *que, task_t* result);


//唤醒阻塞队列
int broadcastAll(list_queue_t *que);



#endif
