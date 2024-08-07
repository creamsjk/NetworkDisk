/*************************************************************************
    > File Name: client.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 21时13分46秒
 ************************************************************************/
#ifndef  __CLIENT_H
#define  __CLIENT_H

#include"../type.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include<unistd.h>



typedef  struct user_s{

   char user[6];
   char password[33];

}user_t;

int transfer_cmd(char *cmd){
    
    if(strcmp(cmd, "pwd") == 0 || strcmp(cmd, "pwd\n") == 0)
        return 1;
    else if(strcmp(cmd, "ls") == 0 || strcmp(cmd, "ls\n") == 0)
        return 2;
    else if(strcmp(cmd, "cd") == 0 || strcmp(cmd, "cd\n") == 0)
        return 3;
    else if(strcmp(cmd, "mkdir") == 0 || strcmp(cmd, "mkdir\n") == 0)
        return 4;
    else if(strcmp(cmd, "rmdir") == 0 || strcmp(cmd, "rmdir\n") == 0)
        return 5;
    else if(strcmp(cmd, "puts") == 0 || strcmp(cmd, "puts\n") == 0)
        return 6;
    else if(strcmp(cmd, "gets") == 0 || strcmp(cmd, "gets\n") == 0)
        return 7;
    else if(strcmp(cmd, "quit") == 0 || strcmp(cmd, "quit\n") == 0)
        return 8;
    else 
        return 9;
}
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



//从服务端下载东西  已经增加断点传输   
int cmd_gets(int peerfd, char *path_name){

    int clientfd = peerfd;

    //进行数据的接受

    char name[256]={0};
    char buff[1024];
    memset(buff,0,sizeof(buff));
    memset(name,0,sizeof(name));

    int len = 0;
    int ret = recv(clientfd,&len,sizeof(len),0);
    printf("len = %d \n",len);

    ret = recv(clientfd,name,len,0);
    int wfd = open(name,O_RDWR | O_CREAT, 0664);
    if(wfd == -1){
        printf("open failed \n");
        exit(1);
    }
    
    //发送文件偏移位置  对面也偏移相同位置  发送一个long

     int file_len = lseek(wfd, 0, SEEK_END);
     send(clientfd, &file_len, sizeof(file_len), 0);
     int cur_len =  lseek(wfd, 0, SEEK_CUR);
     printf("file_len == %d  cur_len == %d   \n", file_len, cur_len);

     //  while(1);



    //开始写文件

    int total =-1;
    recvn(clientfd,&total,sizeof(total));
    printf("total = %d \n",total);

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

        if(len - lastSize > bar){
            printf(" %5f%%  ",(double)100 * len / total);
            int n = len / (bar*10);
            printf("<= ");
            for(int i=0;i<n;i++){
                printf("#");
            }
            printf(" \r");
            //别忘了
            fflush(stdout);
            lastSize = len;
        }

        len += ret;
    }

    return 0;

}

//调用gets 两边都是gets  puts 两边都是puts                                                      
int cmd_puts(int peerfd, char *path_name){

    //读取数据发送到客户端 fd 是客户端文件描述符
    int fd = peerfd;

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
    printf("send firse %d \n",4+srcMessage.len);

    struct stat st;
    fstat(rfd,&st);
    //all bits
    int total = st.st_size;
    int send_bit = 0;
    int len = send(fd,&total,sizeof(total),0);
    printf("send total success  \n");

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
