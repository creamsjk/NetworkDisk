#include "server.h"
#include"../log/log.h"
#include"../thread_cmd/thread_pool.h"
#define MAXSIZE 256

int exitPipe[2] = { 0 };
//保存当前与服务器建立连接用户信息
client_List clientList = { 0 };

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

    //创建日志对象  并初始化  
     log_t *log = (log_t*)malloc(sizeof(log_t));
     log_init(log);
     //需要改变的 日志消息
     char log_message[1024] = {0};

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
        //thread_num为需要启动线程个数
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

        //创建线程 初始化线程
        threadpool_t  *pool = (threadpool_t*)malloc(sizeof(threadpool_t));
        assert(pool);
        //暂时创建五个线程   后续可以更改成形参
        threadpoolInit(pool, atoi(thread_num));

        while(1) {
            ready_num = epoll_wait(epfd, events, 100, -1);
            ERROR_CHECK(ready_num, -1, "epoll_wait");
            for(int i = 0; i < ready_num; i++){
                int fd = events[i].data.fd;
                //有新连接请求
                if(fd == listenfd){
                    //请求建立连接客户端信息
                    struct sockaddr_in clientAdd;
                    socklen_t len = sizeof(clientAdd); 
                    memset(&clientAdd, 0, sizeof(clientAdd));
                    int peerfd = accept(listenfd, (struct sockaddr*)&clientAdd, &len);

                    //验证客户端发过来用户密码(用户密码放在一个结构体之中)
                    //失败直接从continue


           



                    //将新建立连接加入监听   成功
                    epollAddReadEvent(epfd, peerfd);
                    //写入日志  message字符串 进行组装  
                    // write_log(log, message);
                    //写入客户端ip、端口、以及连接时间
                    char message[MAXSIZE] = { 0 };
                    sprintf(message, "client:ip:%s port:%d", inet_ntoa(clientAdd.sin_addr), ntohs(clientAdd.sin_port));
                    write_log(log, message);
                    //添加用户队列  用数组 实现吧 -1 代表就是可以使用的数组
                    client_t* pClient = (client_t*)calloc(1, sizeof(client_t));
                    pClient->m_peerfd = peerfd;
                    strcpy(pClient->m_ip, inet_ntoa(clientAdd.sin_addr));
                    sprintf(pClient->m_port, "%d", ntohl(clientAdd.sin_port));
                    strcpy(pClient->m_pwd, "/");

                    if(clientList.clientSize == 0) {
                        clientList.pFront = clientList.pRear = pClient;
                    }else {
                        clientList.pRear->pNext = pClient;
                        clientList.pRear = pClient;
                    }
                    clientList.clientSize++;

                }else if(fd == exitPipe[0]){
                    //父进程通知子进程退出
                    int exitFlag;
                    read(exitPipe[0], &exitFlag, sizeof(exitFlag));   
                        
                    //子线程停止
                     threadpoolStop(pool);
                     threadpoolDestory(pool);
                    
                }else {
                    //子进程接收到客户端指令，执行指令  
                    //将收到的命令 以及 内容封装 成一个结构体放入阻塞队列中
                    //工作线程 会处理 
                    
                    //通过接受到的命令 以及内容  来初始化task 放入阻塞队列
                    task_t task;
                    memset(&task, 0, sizeof(task));
                    task.m_peerfd = fd;
                    task.m_cmd = (cmd_type)recvCommand(fd, task.m_buff);
                    strcpy(task.m_pwd, "/");
                    taskEnque(pool->m_que, task);
                    //executeCommnd();
                }

            }

        }
    }
    if(pid != 0){
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
