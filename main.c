#include "server.h"
#define MAXSIZE 256

int main(int argc, char* argv[]){
    //从server.conf中读取数据启动服务器
    // ./server server.conf
    ARGS_CHECK(argc, 2);

    char ip[MAXSIZE] = { 0 }; 
    char port[MAXSIZE] = { 0 };
    char thread_num[10] = { 0 };

    //读取配置文件
    int ret = get_config(argv[1], ip, port, thread_num);
    ERROR_CHECK(ret, -1, "get_config");
    //printf("ip:%s port:%s thread_num:%s\n", ip, port, thread_num);

    //创建套接字
    int listenfd = tcp_init(ip, port);
    ERROR_CHECK(listenfd, -1, "tcp_init");

    //初始化epoll对象
    int epfd = epoll_create(0);

    //监听listenfd
    epollAddReadEvent(epfd, listenfd);
    struct epoll_event events[100] =  { 0 };
    int ready_num;

    while(1) {
        ready_num = epoll_wait(epfd, events, 100, -1);
        ERROR_CHECK(ready_num, -1, "epoll_wait");
        for(int i = 0; i < ready_num; i++){
            int fd = events[i].data.fd;
            //有新连接请求
            if(fd == listenfd){
                struct sockaddr_in clientAdd;
                socklen_t len = sizeof(clientAdd); 
                memset(&clientAdd, 0, sizeof(clientAdd));
                int peerfd = accept(listenfd, (struct sockaddr*)&clientAdd, &len);
            }


        }
        
    }

    return 0;
}
