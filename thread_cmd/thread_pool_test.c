/*************************************************************************
    > File Name: thread_pool_test.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 10时30分40秒

    gcc thread_pool.c queue_thread.c   thread_pool_test.c  -o thread_test  -pthread
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include"thread_pool.h"
#include<string.h>
#include<assert.h>
#include<unistd.h>


int main(void){

    threadpool_t  *pool = (threadpool_t*)malloc(sizeof(threadpool_t));
    assert(pool);
    int ret = threadpoolInit(pool,4);

    threadpoolStart(pool);
   task_t result ;
     result.m_cmd = CMD_TYPE_LS;
     result.m_peerfd = 5;
     strcpy(result.m_buff,"nihao");
     if(pool->m_que == NULL )
         printf("asdasd\n");


     for(int i=0;i<10;i++){
      taskEnque(pool->m_que,result);

    }
   
    threadpoolStop(pool);
   // sleep(2);

    threadpoolDestory(pool);

    printf("测试完成\n");

}
