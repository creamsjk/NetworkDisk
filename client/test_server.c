/*************************************************************************
    > File Name: test_server.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月06日 星期二 14时06分41秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<errno.h>
#include<error.h>
#include<arpa/inet.h>
#include"client.h"

//最大监听数量
#define LISTENSOCKET 5
#define MAXSIZE 1024

int main(int argc, char* argv[]){

    //校验参数
    if(argc != 2){
        printf("Usage:. %s  <port> \n",argv[0]);
        exit(-1);
    }

    int server_sock;
    int clnt_sock;
    int str_len;

    struct sockaddr_in server_addr;
    struct sockaddr_in clnt_addr;

    socklen_t clnt_addr_size;

    char message[MAXSIZE];

    //绑定套接字
    server_sock = socket(PF_INET,SOCK_STREAM,0);
    if(server_sock == -1){
        printf("socket failed \n");
        exit(-1);
    }
    int on = 1;
    int ret = setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    //设置地址
    memset(&server_addr,0,sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    //绑定地址
    if(-1 == bind(server_sock,(struct sockaddr *)&server_addr,sizeof(server_addr))){
        printf("bind failed \n");
        exit(-1);
    }

    //监听
    if(-1 == listen(server_sock,LISTENSOCKET)){
        printf("listen failed \n");
        exit(-1);
    }

        clnt_sock = accept(server_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock == -1){
            printf("accept failed \n");
            exit(-1);
        }
        else{
            printf("connect client  \n");
        }

        //接受账号密码  并返回
        user_t user;
        recv(clnt_sock, &user, sizeof(user), 0);

        printf("user =%s   password =%s  \n",user.user, user.password);

        send(clnt_sock, "ok", 3, 0);

        //接受pwd 请求 返回工作目录
        client_cmd_t  cmd;
        recv(clnt_sock, &cmd, sizeof(cmd), 0);
        printf("cmd ==%d  buff=%s \n",cmd.m_cmd, cmd.m_buff);

        char buff[1024];
        memset(buff,'\0', sizeof(buff));
        strcpy(buff,"/home");

        send(clnt_sock, buff, sizeof(buff), 0);
        printf("buff==%s  send success \n",buff);

        //while(1);


        while(1){

        
        memset(buff,'\0', sizeof(buff));
        recv(clnt_sock, &cmd, sizeof(cmd), 0);
        printf("cmd ==%d  buff=%s \n",cmd.m_cmd, cmd.m_buff);

        memset(buff,'\0', sizeof(buff));
        strcpy(buff, "ok\0");
        send(clnt_sock, buff, sizeof(buff), 0);
        printf("buff==%s  send success \n",buff);
        }
        while(1);

        //结束服务器任务
    close(server_sock);

    return 0;
}
