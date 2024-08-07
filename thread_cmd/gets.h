/*************************************************************************
    > File Name: gets.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月06日 星期二 21时23分09秒
 ************************************************************************/

#ifndef  __GETS_H
#define  __GETS_H

#include"../type.h"
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<unistd.h>



int cmd_gets(task_t result, char *path_name){

    //读取数据发送到客户端 fd 是客户端文件描述符
    int fd = result.m_peerfd;

    int rfd = open(path_name,O_RDONLY);
    if(rfd == -1){

        printf("openfile is failed \n");
        return 1;
    }

    message_t srcMessage;
    memset(&srcMessage,0,sizeof(srcMessage));

    //传输文件名
    strcpy(srcMessage.buff,path_name);
    srcMessage.len = strlen(srcMessage.buff);

    //发送全部4 + buff 的内容
    int ret = send(fd,&srcMessage,4+srcMessage.len,0);
    // printf("send firse %d \n",4+srcMessage.len);

    struct stat st;
    fstat(rfd,&st);
    //all bits
    int total = st.st_size;
    int send_bit = 0;
    int len = send(fd,&total,sizeof(total),0);
    //printf("send total success  \n");


    //接受文件偏移量 并且偏移到指定位置  从读的文件描述符中偏移 而不是写中
    int file_len = 0;
    recv(fd, &file_len, sizeof(file_len), MSG_WAITALL);
    int cur_len = lseek(rfd, file_len, SEEK_SET);

    //   printf("file_len == %d  cur_len == %d\n", file_len, cur_len);
  
    //while(1);

 



    while(send_bit < total){
        memset(&srcMessage,0,sizeof(srcMessage));

        //从文件中读一些
        len = read(rfd,srcMessage.buff,sizeof(srcMessage.buff));
       // printf("len == %d \n",len);
        srcMessage.len = len;
        if(srcMessage.len <=0 )
            break;
       // printf("srcMessage.len  =  %d  %s \n",srcMessage.len,srcMessage.buff);

        //分两次发送 第一次发送的是这次发送的文件大小 第二次是真正的文件
        ret = send(fd,&srcMessage.len,sizeof(srcMessage.len),0);

        ret = send(fd,srcMessage.buff,srcMessage.len,0);

        send_bit += ret;
    }

    return 0;
}





#endif  
