#include "server.h"
#define MAXSIZE 256

//ip地址为点分十进制表示
int get_config(char* filename, char* ip, char* port, char* thread_num){
    FILE* fd = fopen(filename, "rb");
    if(!fd) {
        fprintf(stderr, "open %s failed\n", filename);
        exit(1);
    }

    char buf[MAXSIZE] = { 0 };
    int n;
    while((n = fread(buf, sizeof(char), MAXSIZE, fd)) > 0 );
    //printf("buf:%s\n", buf);

    for(int i = 0, j, k = 0; buf[i]; i++){
        if(buf[i] == '=') {
            j = i;
            while(buf[j] != '\n' && buf[j] != '\0'){
                j++;
            }
            switch(k){
            case 0:
                //将ip地址分离出来
                strncpy(ip, buf + i + 1, j - i -1);
                break;
            case 1:
                strncpy(port, buf + i + 1, j - i -1);
                break;
            case 2:
                strncpy(thread_num, buf + i + 1, j - i -1);
                break;
            }
            k++;
            i = j + 1;
        }
    }    

    fclose(fd);
    return 0;
}

//初始化套接字
int tcp_init(const char* ip, const char* port){
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(listenfd, -1, "socket");

    //绑定ip地址
    struct sockaddr_in serverAdd;
    socklen_t len = sizeof(serverAdd);
    memset(&serverAdd, 0, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(atoi(port));
    serverAdd.sin_addr.s_addr = inet_addr(ip);
    int ret = bind(listenfd, (struct sockaddr*)&serverAdd, len);
    ERROR_CHECK(ret, -1, "bind");

    //启动监听,最大支持11个连接
    ret = listen(listenfd, 10);
    ERROR_CHECK(ret, -1, "listenfd");

    //返回套接字
    return listenfd;
}

int epollAddReadEvent(int epfd, int fd){
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd ,&ev);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}

int epollDelReadEvent(int epfd, int fd){
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    ev.events = EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd ,&ev);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}

int executeCommnd(task_t* task){
    //执行客户端传输过来的指令
    //TODO
}