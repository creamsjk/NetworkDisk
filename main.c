#include "server.h"
#define MAXSIZE 256

int exitPipe[2] = { 0 };

void signalhander(int sigNum){
    printf("SIGNAL %d comming\n", sigNum);
    //通过管道通知子进程
    int exitFlag = 1;
    write(exitPipe[1], &exitFlag, sizeof(exitFlag));
    return ;
}

int main(int argc, char* argv[]){
    //从server.conf中读取数据启动服务器
    // ./server server.conf
    ARGS_CHECK(argc, 2);

    //创建退出管道
    pipe(exitPipe);
    pid_t pid = fork();
    switch(pid) {
    case -1:
        ERROR_CHECK(pid, -1, "fork");
    case 0:
        //子进程

        //关闭退出管道写端
        close(exitPipe[1]);

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
                    //将新建立连接加入监听
                    epollAddReadEvent(epfd, peerfd);
                    //添加用户队列
                }else if(fd == exitPipe[0]){
                    //父进程通知子进程退出
                    
                }else {
                    //子进程接收到客户端指令，执行指令
                    //executeCommnd();
                }

            }

        }

    default:
        //父进程
        close(exitPipe[0]);
        //注册信号处理函数
        signal(SIGUSR1, signalhander);
        wait(NULL);
        close(exitPipe[1]);
        exit(0);
    }

    return 0;
}
