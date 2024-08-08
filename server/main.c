#include "server.h"
#include"../log/log.h"
#include"../thread_cmd/thread_pool.h"
#include"../my_mysql/my_mysql.h"
#define MAXSIZE 1024

//编译命令  
//gcc main.c server.c  ../thread_cmd/thread_pool.c  ../thread_cmd/queue_thread.c ../my_mysql/my_mysql.c   -o server -pthread -w -lmysqlclient






int exitPipe[2] = { 0 };
//保存当前与服务器建立连接用户信息
client_List clientList;

char * home = "/home/sunrise/桌面/wangdao/NetworkDisk/home";

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
  
     //数据库连接
    MYSQL *pconn = connect_db(); 

     

     //连接队列
     clientList.clientSize = 0;
     clientList.pFront = NULL;
     clientList.pRear = NULL;
     


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

        strcpy(ip,"127.0.0.1");
        strcpy(port,"8080");
        //创建套接字
        int listenfd = tcp_init(ip, port);
        ERROR_CHECK(listenfd, -1, "tcp_init");

        //初始化epoll对象
        int epfd = epoll_create(5);

        //监听listenfd
        //epollAddReadEvent(epfd, listenfd);
        epollAddReadEventServer(epfd, listenfd);
        struct epoll_event events[100] =  { 0 };
        int ready_num;

        //创建线程 初始化线程
        threadpool_t  *pool = (threadpool_t*)malloc(sizeof(threadpool_t));
        assert(pool);
        //暂时创建五个线程   后续可以更改成形参
        threadpoolInit(pool, atoi(thread_num));

        //启动线程池
        threadpoolStart(pool);

       // printf("进入epoll  \n");
       // printf("epfd == %d \n",epfd);

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
                    //接受账号密码  并返回
                    
                    printf("进来一个客户端 \n");

                   
                    user_t user;
                    memset(&user, '\0', sizeof(user));

                    int select_login_register = 0;
                      
                    recv(peerfd, &select_login_register, sizeof(select_login_register), 0);

                    if(select_login_register != 2 &&  select_login_register != 1){
                        continue;
                    }else if (select_login_register == 2){
                        //执行注册
                        ret = recv(peerfd, &user, sizeof(user), 0);
                        ret = insert_user(pconn, user.user,  user.password, "/home/sunrise/桌面/wangdao/NetworkDisk/home");
                        if(ret == -1){
                          send(peerfd, "error", 6, 0);
                          continue;
                        }
                        else 
                          send(peerfd, "ok", 3, 0);  

                    }
            






                   printf("进入密码验证阶段!!\n"); 
                    //while(1);
                    while(1){

                    
                    ret = recv(peerfd, &user, sizeof(user), 0);
                    //printf("ret == %d \n",ret);

                    printf("user =%s|   password =%s|  \n",user.user, user.password);
                    if(find_user_is_exist(pconn, user.user) != 1){     
                        send(peerfd, "error", 6, 0);
                        printf("没找到user \n");
                        continue;

                    }
                    char * usr_pasword =  get_user_password(pconn, user.user);
                    printf("user_password %s \n ",usr_pasword);
                    if(strcmp(usr_pasword, user.password) == 0)  {
                       send(peerfd, "ok", 3, 0);
                       break;
                    }
                    else{
                        send(peerfd, "error", 6, 0);
                        printf("密码不对!! \n");

                    }
                    }

                    //while(1);
                         
                     
                    


                    //将新建立连接加入监听   成功
                    epollAddReadEvent(epfd, peerfd);
                    //写入日志  message字符串 进行组装  
                    // write_log(log, message);
                    //写入客户端ip、端口、以及连接时间
                    char message[MAXSIZE] = { 0 };
                    sprintf(message, "fd: %d   ip:%s   port:%d   connect", peerfd,  inet_ntoa(clientAdd.sin_addr), ntohs(clientAdd.sin_port));
                    //printf("message == %s \n",message);
                    write_log(log, message);

                    //添加用户队列  用数组 实现吧 -1 代表就是可以使用的数组
                    client_t* pClient = (client_t*)calloc(1, sizeof(client_t));
                    pClient->m_peerfd = peerfd;
                    strcpy(pClient->m_ip, inet_ntoa(clientAdd.sin_addr));
                    sprintf(pClient->m_port, "%d", ntohs(clientAdd.sin_port));
                    strcpy(pClient->m_pwd, "/home/sunrise/桌面/wangdao/NetworkDisk/home");
                

                   // printf("fd = %d ip:%s port:%s  pwd:%s \n", pClient->m_peerfd, pClient->m_ip, pClient->m_port, pClient->m_pwd );

                    if(clientList.clientSize == 0) {
                        clientList.pFront = clientList.pRear = pClient;
                    
                        pClient->pNext = NULL;
                    }else {

                        clientList.pRear->pNext = pClient;
                        clientList.pRear = pClient;
                        pClient->pNext = NULL ;
                    }
                    clientList.clientSize++;
                   // printf(" ip:%s  port:%s  \n", pClient->m_ip, pClient->m_port );

                    //printf("clientList.clientSize == %d \n",clientList.clientSize);

                }else if(fd == exitPipe[0]){
                    //父进程通知子进程退出
                    int exitFlag;
                    read(exitPipe[0], &exitFlag, sizeof(exitFlag));   
                        
                    //子线程停止
                     threadpoolStop(pool);
                     threadpoolDestory(pool);

                     //关闭数据库连接
                     close_db(pconn);
                }else if(events[i].events & EPOLLRDHUP){
                     //事件发生 就是客户端关闭 从epoll中将他删除
                     //当然 还要从队列中删除 这个文件描述符  目前没有写
                     
                    char message[MAXSIZE] = { 0 };

                    if(clientList.clientSize == 1){
                        client_t *p = clientList.pFront;
                        clientList.pFront = NULL;
                        clientList.pRear = NULL;
                        if( p != NULL){

                        sprintf(message, "fd: %d   ip:%s   port:%s   disconnect", p->m_peerfd, p->m_ip,  p->m_port);
                        //printf(" message =%s  ip:%s  port:%s  \n", message, p->m_ip, p->m_port );
                        }
                        

                        free(p);
                        p = NULL;
                    }else {
                        //这个文件描述符一定是存在的
                        client_t *pCur = clientList.pFront->pNext;
                        client_t *pPre = clientList.pFront;

                        while(pCur->m_peerfd != fd && pCur->pNext != NULL){
                          pPre = pCur;
                          pCur = pCur->pNext;

                        }

                        pPre->pNext = pCur->pNext;
                        //如果pPre->next == NULL  就说明他是最后一个改变pRear指针
                        if(pPre->pNext == NULL)
                            clientList.pRear = pPre;
                        if(pCur != NULL)
                        sprintf(message, "fd: %d  ip:%s   port:%s   disconnect", pCur->m_peerfd,  pCur->m_ip,  pCur->m_port);

                        free(pCur);
                        pCur = NULL;

                    }
                     
                    // printf("关闭文件描述符:%d \n", fd);

                     // 写日志
                   // printf("message == %s \n",message);
                    write_log(log, message);
                     
                     
                     clientList.clientSize--;
                     epollDelReadEvent(epfd, fd);
                     close(fd);
                     
                    // printf("clientList.clientSize == %d \n", clientList.clientSize);
                    // 测试通过没有什么问题
                
                }else {
                    //子进程接收到客户端指令，执行指令  
                    //将收到的命令 以及 内容封装 成一个结构体放入阻塞队列中
                    //工作线程 会处理 
                     
                     printf("接受客户端发过来的命令 \n");
                    //将客户端发送过来的数据接受
                    client_cmd_t  client_message;
                    ret = recv(fd, &client_message, sizeof(client_message), 0);
                    //printf("ret == %d \n",ret);
                    
                    //通过接受到的命令 以及内容  来初始化task 放入阻塞队列
                    task_t task;
                    memset(&task, 0, sizeof(task));
                    task.m_peerfd = fd;
                    task.m_epfd = epfd;
                    //printf("task.epfd = %d\n", task.m_epfd);
                    //task.m_cmd = (cmd_type)recvCommand(fd, task.m_buff);
                    task.m_cmd = client_message.m_cmd;
                    strcpy(task.m_buff, client_message.m_buff); 

                    //暂时将本项目下 /home  作为所有客户端的家目录
                    strcpy(task.m_pwd, "/home/sunrise/桌面/wangdao/NetworkDisk");
                    strcat(task.m_pwd, client_message.m_pwd);
                    

                    //加入阻塞队列
                    taskEnque(pool->m_que, task);

                    printf("fd = %d  cmd == %d  m_buff = %s  m_pwd = %s  \n", task.m_peerfd, task.m_cmd, task.m_buff, task.m_pwd );
                    //printf("成功加入阻塞队列 \n");
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
