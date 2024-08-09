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
#include"cd.h"
#include"ls.h"
#include"pwd.h"
#include"mkdir.h"
#include"rmdir.h"
#include"gets.h"
#include"puts.h"
#include"../server/server.h"
#include"../my_mysql/my_mysql.h"

char * root = "/home/sunrise/桌面/wangdao/NetworkDisk";

void process(task_t task, MYSQL * pconn){

    // printf("%ld执行任务  over\n",pthread_self());
   // printf("process_pwd == %s \n", task.m_pwd);
    switch(task.m_cmd){
    case CMD_TYPE_PWD:
        {
            //pwd 成功实现 先暂时不要free
            char *result = cmd_pwd(task, root);
            
            //发送给客户端
            //先只写这一个 等待解决其他问题 在写后面其他命令细节
            int len = strlen(result);
            result[len] = '\0';
            send(task.m_peerfd, result, len + 1, 0);
            
            //printf("pwd buff is  %s \n",result);
            //while(1);
            //free(result);
            //
            //epollMod(task.m_epfd, task.m_peerfd);
                          
            //printf("pwd_epfd == %d \n",task.m_epfd);
            break;

        }
    case CMD_TYPE_LS:
        {
            //后边和客户端处理
            printf("work_ls_pwd == %s \n",task.m_pwd);
            char * result = cmd_ls(task, root, pconn);
            int len = strlen(result);
            result[len] = '\0';
            send(task.m_peerfd, result, len + 1, 0);

            //printf("pwd buff is  %s \n",result);


            break;
        }
    case CMD_TYPE_CD:
        {
            char *result = cmd_cd(task, root, pconn);  
           // printf("cd pwd  == %s \n", task.m_pwd);

            int len = strlen(result);
            result[len] = '\0';
            send(task.m_peerfd, result, len + 1, 0);


            //free(h);
            break;
        }
    case CMD_TYPE_MKDIR:
        {

             int ret  = cmd_mkdir(task, root, pconn);
             if(ret == 0)
                send(task.m_peerfd, "ok", 3, 0);
             else
                 send(task.m_peerfd, "error", 6, 0);

            break;
        }
    case CMD_TYPE_RMDIR:
        {
            int ret  = cmd_rmdir(task, root, pconn);

            if(ret == 0)
                send(task.m_peerfd, "ok", 3, 0);
            else
                send(task.m_peerfd, "error", 6, 0);

            break;
        }
    case CMD_TYPE_PUTS:
        {
            printf("puts join \n");

            send(task.m_peerfd, "ok", 3, 0);


            epollDelReadEvent(task.m_epfd, task.m_peerfd);

            char hash_recv[33] = { 0 };
            recv(task.m_peerfd, hash_recv, sizeof(hash_recv), 0);

            printf("recv_hash is  %s \n", hash_recv);

            //查找文件是否在服务器上
            int ret =  find_file_is_exist( pconn, hash_recv);

            //while(1);
               

            char *s = (char *)malloc(sizeof(char) * 200);
            int len = strlen(task.m_buff);
            task.m_buff[len -1] = '\0';
            // strcpy(s, root);
            // strcat(s, "/");
             
            strcat(s, task.m_pwd);
            s = strpbrk(s, "/"); 

           // strcat(s, "/");
           // strcat(s,task.m_buff);

            printf("put_name=%s 1 user=%s \n", s, task.m_user);
            //文件名存在返回1  不存在返回0  失败返回-1
            if(ret == 1){

             ret = insert_file(pconn, task.m_buff, "f", task.m_user, hash_recv, s);

                send(task.m_peerfd, "exists", 7, 0);    
            }else if(ret == 0){

              ret = insert_file(pconn, task.m_buff, "f", task.m_user, hash_recv, s);


             //服务器上不存在的文件发送 ok
              send(task.m_peerfd, "ok", 3, 0);
              cmd_puts(task, s);
              //添加服务器上没有的文件  暂时没有设置文件大小 统一设置成100
              char global_file[200] = { 0 };
              strcpy(global_file, s);
              strcat(global_file, "/");
              strcat(global_file, task.m_buff);
              ret = insert_global_file(pconn, hash_recv, global_file , 100);
              
                
            }else{

                printf("数据库查找失败!! \n");
            }
            

            //strcpy(s, "/home/sunrise/桌面/wangdao/NetworkDisk/home/abc.txt");
            //while(1);
            //
            //数据库给用户添加一个数据
            printf("puts inset file  ret == %d \n",ret);
           // ret = insert_file(pconn, task.m_buff, "f", task.m_user, hash_recv, s);
            

            epollAddReadEvent(task.m_epfd, task.m_peerfd);


            break;

        }
    case CMD_TYPE_GETS:
        {
            
            printf("gets join \n");
            epollDelReadEvent(task.m_epfd, task.m_peerfd);
            char *s = (char *)malloc(sizeof(char) * 200);
            int len = strlen(task.m_buff);
            task.m_buff[len -1] = '\0';
            // strcpy(s, root);
            // strcat(s, "/");
            strcat(s, task.m_pwd);
            strcat(s, "/");
            strcat(s,task.m_buff);
           
            printf("gets name=%s \n", s);
            //strcpy(s, "/home/sunrise/桌面/wangdao/NetworkDisk/home/abc.txt");
            cmd_gets(task, s);


            epollAddReadEvent(task.m_epfd, task.m_peerfd);

             break;
        }
    case CMD_TYPE_QUIT:
        {
           break;
        }
    default:
        break;
    }

}


void *work(void* arg){
    // printf("work .......\n");
    threadpool_t *pool = (threadpool_t* )arg;

    //创建一个属于自己的数据库连接
   MYSQL *pcoon = connect_db(); 

    while(1){
       task_t  tmp;
       //从阻塞队列拿出来以一个
       int ret = taskDeque(pool->m_que,&tmp);
       // printf(" this peerfd  %d  and pwd is %s \n", tmp.m_peerfd, tmp.m_pwd);
       assert(ret == 0);
        int peerfd = tmp.m_peerfd;
       if(peerfd > 0){
         process(tmp, pcoon);

       }else{
           break;
       }

    }
    printf("sub thread %ld is exiting. \n",pthread_self());

    //关闭数据库连接
    close_db(pcoon);
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

