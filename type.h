/*************************************************************************
    > File Name: type.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月03日 星期六 16时47分52秒
 ************************************************************************/

typedef enum cmd_t{
    CMD_TYPE_PWD = 1,
    CMD_TYPE_LS,
    CMD_TYPE_CD,
    CMD_TYPE_MKDIR,
    CMD_TYPE_RMDIR,
    CMD_TYPE_PUTS,
    CMD_TYPE_GETS,
    CMD_TYPE_QUIT,
    CMD_TYPE_NOTCMD


}cmd_type;
 typedef struct task_s{

     //文件描述符
     int m_peerfd;
     //命令
     cmd_type m_cmd;
     //命令附带的参数
     char m_buff[200];
     //客户端的工作目录
     char m_pwd[200];

}task_t;

typedef struct clint_data{

    //客户端文件描述符
    int m_peerfd;
     //ip地址
    char m_ip[100];
     //端口    
    char m_port[20];
     //工作目录
    char m_pwd[200];
}client_t;
