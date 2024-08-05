#ifndef __SERVER_H
#define __SERVER_H 

#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/uio.h>
#include <shadow.h>

#define SIZE(a) (sizeof(a)/sizeof(a[0]))

typedef void (*sighandler_t)(int);

//参数个数检验
#define ARGS_CHECK(argc, num)   {\
    if(argc != num){\
        fprintf(stderr, "ARGS ERROR!\n");\
        return -1;\
    }}

#define ERROR_CHECK(ret, num, msg) {\
    if(ret == num) {\
        perror(msg);\
        return -1;\
    }}

#define THREAD_ERROR_CHECK(ret, func) {\
    if(ret != 0) {\
        fprintf(stderr, "%s:%s\n", func, strerror(ret));\
        return -1;\
    }}

//从配置文件中读取配置信息
int get_config(char*, char*, char*, char*);

//创建套接字
int tcp_init(const char* ip, const char* port);

//封装epoll函数
int epollAddReadEvent(int epfd, int fd);
int epollDelReadEvent(int epfd, int fd);

int executeCommand(task_t*);

//用户登录校验
char* userLoginCheck1(const char* username);
int  userLoginCheck2(const char* encrypt);

int recvCommand(int peerfd, char* buf);

#endif
