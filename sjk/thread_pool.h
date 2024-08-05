/*************************************************************************
    > File Name: thread_pool.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 10时01分40秒
 ************************************************************************/

#ifndef  __PTHREAD_POOL_H
#define  __PTHREAD_POOL_H

#include"queue_thread.h"

typedef struct threadpool_s{
    //线程
    pthread_t *m_pthreads;
    //最大线程数
    int m_pthreadNum;
    //阻塞队列
    list_queue_t * m_que;

}threadpool_t;

//初始化线程池
int threadpoolInit(threadpool_t *pool,int num);

//销毁线程池
int threadpoolDestory(threadpool_t *pool);

//启动线程池
int threadpoolStart(threadpool_t *pool);

//停止线程池
int threadpoolStop(threadpool_t *pool);


#endif
