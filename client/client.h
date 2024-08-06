/*************************************************************************
    > File Name: client.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 21时13分46秒
 ************************************************************************/
#ifndef  __CLIENT_H
#define  __CLIENT_H

#include<string.h>

typedef  struct user_s{

   char user[6];
   char password[33];

}user_t;

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

//客户端发送的数据
typedef struct client_cmd{

     //命令
     cmd_type m_cmd;
     //命令附带的参数
     char m_buff[200];


}client_cmd_t;

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


#endif
