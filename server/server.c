#include "server.h"
#include "../type.h"
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

    //设置端口复用
    int on = 1;
    int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


    //绑定ip地址
    struct sockaddr_in serverAdd;
    socklen_t len = sizeof(serverAdd);
    memset(&serverAdd, 0, sizeof(serverAdd));
    serverAdd.sin_family = AF_INET;
    serverAdd.sin_port = htons(atoi(port));
    serverAdd.sin_addr.s_addr = inet_addr(ip);
    ret = bind(listenfd, (struct sockaddr*)&serverAdd, len);
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
   // ev.events = EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd ,&ev);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}

int epollDelReadEvent(int epfd, int fd){
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.fd = fd;
    //ev.events = EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    int ret = epoll_ctl(epfd, EPOLL_CTL_DEL, fd ,&ev);
    ERROR_CHECK(ret, -1, "epoll_ctl");

    return 0;
}

int executeCommnd(task_t* task){
    //执行客户端传输过来的指令
    //TODO
}

//返回盐值地址                                                                                                                                                    
char* userLoginCheck1(const char* username){
    //通过用户名获取用户salt值
    struct spwd* pSpwd = (struct spwd*) calloc(1, sizeof(struct spwd));
    pSpwd = getspnam(username);
    //printf("encrypted password: %s\n", pSpwd->sp_pwdp);

    char* buf = (char*)calloc(MAXSIZE, sizeof(char));
    struct spwd *temp = pSpwd;
    for(int i = 0, j = 0; temp->sp_pwdp[i]; i++){
        if(temp->sp_pwdp[i] == '$'){
            j++;
        }
        //第三个$之后
        if(j == 3){
            int k = i + 1;
            while(temp->sp_pwdp[k] != ':'){
                k++;
            }
            strncpy(buf, temp->sp_pwdp + i + 1, k - i - 1);
            break;
        } 
    }

    //printf("salt: %s\n", buf);
    return buf;
}  

//将客户端传送来的密文与shaow中的密文做对比
//encrypt为用户传递的加密密文，username为该用户登录名
int userLoginCheck2(const char* encrypt, const char* username){
    struct spwd* pSpwd = (struct spwd*) calloc(1, sizeof(struct spwd));
    pSpwd = getspnam(username);

    char buf[MAXSIZE] = { 0 };
    struct spwd *temp = pSpwd;
    for(int i = 0, j = 0; temp->sp_pwdp[i]; i++){
        if(temp->sp_pwdp[i] == '$'){
            j++;
        }
        //第四个$之后的密文
        if(j == 4){
            int k = i + 1;
            while(temp->sp_pwdp[k] != ':'){
                k++;
            }
            strncpy(buf, temp->sp_pwdp + i + 1, k - i - 1);
            break;
        } 
    }
    
    int ret = strcmp(encrypt, buf);
    return !ret;
}

//按(length, cmdType, content)类型接收客户端数据
//返回指令类型，buf为指令参数
int recvCommand(int peerfd, char* buf) {
    //先获取数据长度
    int length;
    recv(peerfd, &length, sizeof(length), MSG_WAITALL);

    //enum类型为4字节大小
    cmd_type cmd;
    recv(peerfd, &cmd, sizeof(cmd), MSG_WAITALL);
    
    if(cmd = CMD_TYPE_NOTCMD){
        return -1;
    }
    recv(peerfd, buf, length, MSG_WAITALL);

    return (int)cmd;
}

//生成随机字符串（salt值）
//n为生成salt值位数
char* generateSalt(int n){
    int flag, i;
    char* salt;
    srand((unsigned) time(NULL ));
    if ((salt= (char*) malloc(n)) == NULL ) {
        fprintf(stderr, "malloc failed\n");
        return NULL ;
    }
    for (i = 0; i < n+1; i++) {
        flag = rand() % 3;
        switch (flag) {
        case 0: salt[i] = 'A' + rand() % 26; break;
        case 1: salt[i] = 'a' + rand() % 26; break;
        case 2: salt[i] = '0' + rand() % 10; break;
        default: salt[i] = 'x'; break;
        }
    } 
    //生成随机字符串末尾加上'\0'
    salt[n] = '\0';
    return salt;
}
