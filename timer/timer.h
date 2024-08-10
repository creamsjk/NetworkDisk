/*************************************************************************
    > File Name: timer.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月10日 星期六 00时56分50秒
 ************************************************************************/
#ifndef __TIME_H
#define __TIME_H


#include"hashtable.h"


//单链表
typedef struct  timer_s{

    int fd;
    struct timer_s * pNext;

}my_time_t;

typedef  struct tin_s{

    my_time_t * solt;
}my_timer;

//删除time 指向的为 fd 的定时器 time 不为空
void delete_timer(int fd, my_time_t * time){

    //能进入这里 就代表fd 一定存在这里面
    //time->pNext  就是第一个上面的节点
    my_time_t * pCur = time->pNext;
    my_time_t * pPre = time;

    while(pCur != NULL || time == NULL){

        if(pCur->fd == fd){
        
            pPre->pNext = pCur->pNext;
            pCur->fd = -1;
            free(pCur);

            printf(" 删除%d成功 \n", fd);
            break;
            
        }
        pPre = pCur;
        pCur = pCur->pNext;

    } 


}

//添加fd 的定时器
void  add_timer(int fd, my_time_t *time){

    //time一定不为空
    my_time_t * p = time;

    while(p->pNext != NULL){
        p = p->pNext;
    }
   my_time_t * node = (my_time_t*)malloc( sizeof(my_time_t));
   node->fd = fd;
   p->pNext = node;
   node->pNext = NULL;

   printf("添加fd=%d \n",node->fd);

}

void show_timer(my_time_t * time){

    my_time_t * p = time->pNext;

    while( p != NULL){

        printf("fd is %d \n",p->fd);
        p = p->pNext;
    }
        
}

//返回一个fd 知道time->pNext为NULL return -1 表示这个槽取完了 结束
int take_timer(my_time_t *time){

    
    if(time == NULL || time->pNext == NULL)
        return -1;

    my_time_t * p = time->pNext;
    time->pNext = p->pNext;

    return p->fd;
}


#endif




