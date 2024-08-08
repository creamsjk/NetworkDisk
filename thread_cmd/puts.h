/*************************************************************************
  > File Name: puts.h
  > Author: sunrie
  > Mail: 1102490208@qq.com 
  > Created Time: 2024年08月07日 星期三 09时12分26秒
  
  测试通过
 ************************************************************************/

#ifndef  __PUTS_H
#define  __PUTS_H

#include"../type.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include <unistd.h>
#include<string.h>
#include<sys/stat.h>

int recvn(int sockfd, void* buff, int len);

//接收确定的字节数的数据 等接受到注定数量的数据在结束
int recvn(int sockfd, void * buff, int len)
{
    int left = len;
    char * pbuf =(char *) buff;
    int ret = -1;
    while(left > 0) {
        ret = recv(sockfd, pbuf, left, 0);
        if(ret == 0) {
            break;
        } else if(ret < 0) {
            perror("recv");
            return -1;
        }
        left -= ret;
        pbuf += ret;
    }
    return len - left;
}


int cmd_puts(task_t task, char *path_name){

    int clientfd = task.m_peerfd;

    //进行数据的接受

    char name[256]={0};
    char buff[1024];
    memset(buff,0,sizeof(buff));
    memset(name,0,sizeof(name));

    int len = 0;
    int ret = recv(clientfd,&len,sizeof(len),0);
    //printf("len = %d \n",len);
    if(len == -1)
        return 1;

    ret = recv(clientfd,name,len,0);
    int wfd = open(name,O_RDWR | O_CREAT, 0664);
    if(wfd == -1){
        printf("open failed \n");
        exit(1);
    }


    //开始写文件

    int total =-1;
    recvn(clientfd,&total,sizeof(total));
    //printf("total = %d \n",total);

    len = 0;
    int tmp_len=0;
    //printf("len = %d total = %d \n",len,total);
    //进度条设置
    int bar = total / 100;
    int lastSize = 0;
    while(len < total){
        memset(buff,0,sizeof(buff));
        ret = recvn(clientfd,&tmp_len,sizeof(tmp_len));
        // printf("tmp_len = %d \n",tmp_len);
        ret = recvn(clientfd,buff,tmp_len);

        if(ret <=0 )
            break;
        //  printf("ret = %d \n",ret);
        //  printf("buff = %s \n",buff);

        write(wfd,buff,tmp_len);

       // if(len - lastSize > bar){
       //     printf(" %5f%%  ",(double)100 * len / total);
       //     int n = len / (bar*10);
       //     printf("<= ");
       //     for(int i=0;i<n;i++){
       //         printf("#");
       //     }
       //     printf(" \r");
       //     //别忘了
       //     fflush(stdout);
       //     lastSize = len;
       // }

        len += ret;
    }
    printf("puts完成!! \n");

    return 0;
}







#endif 
