/*************************************************************************
    > File Name: client_main.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月06日 星期二 09时07分21秒

    gcc client_main.c -o client  -lcrypto
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include"client.h"
#include"md5.h"
#include<assert.h>

int main(void){
    
    //创建客户端的套接字
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    //指定使用的是IPv4的地址类型 AF_INET
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8080);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //连接服务器
    int ret = connect(clientfd, (struct sockaddr*)&serveraddr,
                      sizeof(serveraddr));

     
    
    //验证账号 密码
    user_t client;
    memset(&client, '\0',sizeof(client));
    int recv_len = -1;
    char log_in_message[16] = {0};
    char log_register[16] = { 0 };

    //md5 加密
    unsigned char md[16] = {0};
    char result[32] = {0};
      
    char password[20]= {0};
    int select_login_register = 0;
    

    printf("Please select login  or  register \n");
    printf("Enter 1 is login \n");
    printf("Enter 2 is register \n");
    scanf("%d", &select_login_register);

    //printf("select_login_register ==%d| \n", select_login_register);
    send(clientfd, &select_login_register, sizeof(int), 0);

    if(select_login_register != 1 &&  select_login_register != 2){
        
              printf("\033[0m\033[1;31m bye!! \033[0m\n");
              return 1;
    }


    //注册
    if(select_login_register == 2){

        printf("Please inter your register  user: ");
        scanf("%s",client.user);

        printf("Please inter your register  password: ");
        scanf("%s",password);


        if(strcmp(client.user, "quit") == 0 || strcmp(client.password, "quit") == 0){
            
              printf("\033[0m\033[1;31m bye!! \033[0m\n");
              return 1;
         
        }

        strcpy(result, password);
        md5_to((unsigned char *)password, sizeof(password), md, result);

        strcpy(client.password, result);


        send(clientfd, &client, sizeof(client), 0);

        recv_len = recv(clientfd,  log_register, sizeof(log_register), 0);
        
        if(strcmp(log_register, "error") == 0){
                 
              printf("\033[0m\033[1;31m user is failed bye!! \033[0m\n");
              return 1;
            
     
        }
        printf("register is success \n");

    }

   //while(1); 
   printf("进入输入密码阶段!!\n");

    while(strcmp(log_in_message, "ok") != 0){

        if(recv_len >= 0 ){

            printf("\033[0m\033[1;31m user or password  failed  \033[0m\n");
            //select_login_register = 1;
            //send(clientfd, &select_login_register, sizeof(int), 0);
        }

        printf("Please inter your user: ");
        scanf("%s",client.user);

        printf("Please inter your password: ");
        scanf("%s",password);


        if(strcmp(client.user, "quit") == 0 || strcmp(client.password, "quit") == 0){
            
              printf("\033[0m\033[1;31m bye!! \033[0m\n");
              return 1;
         
        }

        strcpy(result, password);
        md5_to((unsigned char *)password, sizeof(password), md, result);

        strcpy(client.password, result);


        send(clientfd, &client, sizeof(client), 0);

        recv_len = recv(clientfd,  log_in_message, sizeof(log_in_message), 0);
    }

    printf("\033[0m\033[1;31m Enter Server  \033[0m\n");


     //通过之后发个pwd  用户来看自己位置
     char pwd[200] = {0};
     client_cmd_t cmd;
     cmd.m_cmd = CMD_TYPE_PWD;
     memset(&cmd.m_buff, '\0', sizeof(cmd.m_buff));
     memset(&cmd.m_pwd, '\0', sizeof(cmd.m_pwd));

     //初始目录
     strcpy(cmd.m_pwd, "/home");
     

     char buff[10240];
     send(clientfd, &cmd, sizeof(cmd), 0);
     memset(buff, '\0', sizeof(buff));
      ret = recv(clientfd, buff, sizeof(buff), 0);

      //更新位置
      strcpy(pwd, buff);  
      


    printf("connect success.\n");

    //使用select  进行数据的接收和发送

    fd_set rdset;
    FD_ZERO(&rdset);
    //char buff[100] = {0};
    FD_SET(STDIN_FILENO, &rdset);
    FD_SET(clientfd, &rdset);
    fd_set tmpset;
    FD_ZERO(&tmpset);

    //复制文件描述符 用来操作puts 和gets
    int newfd = dup(clientfd);

    //标志位 表示退出客户端
    int quit = 0;

    

    //事件循环
    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);
    fflush(stdout);
    while(1) {
        
         tmpset = rdset; 
        select(clientfd + 1, &tmpset, NULL, NULL, NULL);
        //当select函数返回时，rdset会被修改的

        if(FD_ISSET(STDIN_FILENO, &tmpset)) {

            memset(buff, 0, sizeof(buff));
            //通过read函数会把'\n'也读进来
            ret = read(STDIN_FILENO, buff, sizeof(buff));

            //处理标准输入分割成 cmd_type  和  m_buff
            //  int tmp  = (int)send_cmd - 48;
            //  cmd.m_cmd = (cmd_type)tmp;
           char * send_cmd;
                int i = 0;  
                            
                    while(buff[i] == ' ')
                        i++;                 
                                             
                send_cmd = buff + i;       
                //printf("%s ",send_cmd);   
                                             
                while(buff[i] != '\0' &&  buff[i++] != ' ')
                     ;      
                if(buff[i] != '\0')
                   buff[i-1]='\0';
                         
                while(buff[i] == ' ')
                     i++;
                         
                         
               char *send_buff = buff  + i;
                int tmp  = transfer_cmd(send_cmd);
               // printf("tmp == %d \n",tmp);
               // printf("send_buff =%s \n",send_buff);
               
            


            //进行命令组装发送  不带参数的就先发ok
            int send_flag = 0;
            switch(tmp){
                
            case 1 :
                {
                    cmd.m_cmd = CMD_TYPE_PWD;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff, "ok");
                    break;
                }
            case 2:
                {
                    cmd.m_cmd = CMD_TYPE_LS;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff, "ok");

                    break;
                }
            case 3:
                {
                    cmd.m_cmd = CMD_TYPE_CD;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff, send_buff);
                    // printf("cd send =%s \n",cmd.m_buff);


                    break;
                }
            case 4:
                {
                    cmd.m_cmd = CMD_TYPE_MKDIR;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff, send_buff);

                    break;
                }
            case 5:
                {
                    cmd.m_cmd = CMD_TYPE_RMDIR;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff,  send_buff);

                    break;
                }
            case 6:
                {
                    cmd.m_cmd = CMD_TYPE_PUTS;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff,  send_buff);

                    break;
                }
            case 7:
                {
                    cmd.m_cmd = CMD_TYPE_GETS;
                    memset(cmd.m_buff, '\0', sizeof(cmd.m_buff));
                    strcpy(cmd.m_buff,  send_buff);

                    break;
                }
            case 8:
                {
                    cmd.m_cmd = CMD_TYPE_QUIT;
                    quit = 1;
                    break;
                }
             default:
                 printf(" send  error  cmd \n");
                 send_flag = 1;
                 printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

            }
           // strcpy(cmd.m_buff, send_buff);
          
            //此时 已经组装好命令 发送命令 
            fflush(stdout);
          if(send_flag == 0)
            send(clientfd, &cmd, sizeof(cmd), 0);
           // printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);
             


                      
        }

        
        if(FD_ISSET(clientfd, &tmpset)) {

            //从服务器接收数据
            memset(buff, 0, sizeof(buff));

            //等待接收数据  给客户端发送什么样的数据 就说明 想接受什么数据
            //判断接收行为
            int tmp = (int)cmd.m_cmd;
            switch(tmp){

            case 1 :
                {
                    cmd.m_cmd = CMD_TYPE_PWD;         

                    //printf("%s @ %s : ", client.user, pwd);
                 //   printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

                    ret = recv(clientfd, buff, sizeof(buff), 0);
                    if(ret <= 0)
                        quit = 1;
                    
                    printf("%s \n",buff);

                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

                    break;
                }
            case 2:
                {
                    cmd.m_cmd = CMD_TYPE_LS;
                    //printf("%s @ %s : ", client.user, pwd);
                   //  printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

                    ret = recv(clientfd, buff, sizeof(buff), 0);
                    if(ret <= 0)
                        quit = 1;
                    
                    printf("%s \n",buff);

                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

                    break;
                }
            case 3:
                {
                    //cd 更改客户端命令
                    cmd.m_cmd = CMD_TYPE_CD;
                    ret = recv(clientfd, buff, sizeof(buff), 0);

                    if(ret <= 0)
                         quit = 1;

                    printf(" %s \n",buff);

                    if(strcmp(buff, "error") == 0){
                       printf("that is not file !! \n");
                       //while(1);
                    }else{

                        strcpy(pwd,buff);
                        int len = strlen(pwd);
                        pwd[len]='\0';
                    }
                    strcpy(cmd.m_pwd, pwd);

                   // printf("%s @ %s : ", client.user, pwd);
                   
                    //printf("pwd_cd == %s \n", pwd);
                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);
                    

                    break;
                }
            case 4:
                {
                    cmd.m_cmd = CMD_TYPE_MKDIR;
                    ret = recv(clientfd, buff, sizeof(buff), 0);

                    if(ret <= 0)
                        quit = 1;

                    if(strcmp(buff, "ok") == 0)
                        printf("mkdir ok \n");
                    else 
                        printf("mkdir error \n");
                   // printf("%s @ %s : ", client.user, pwd);
                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);
                     
                    break;
                }
            case 5:
                {
                    cmd.m_cmd = CMD_TYPE_RMDIR;
                    ret = recv(clientfd, buff, sizeof(buff), 0);
                    if(ret <= 0)
                        quit = 1;

                    if(strcmp(buff, "ok") == 0)
                        printf("rmdir ok \n");
                    else 
                        printf("rmdir error \n");


                    //printf("%s @ %s : ", client.user, pwd);
                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

                    break;
                }
            case 6:
                {
                    cmd.m_cmd = CMD_TYPE_PUTS;
                    
                    ret = recv(clientfd, buff, sizeof(buff), 0);
                    if(ret <= 0)
                        quit = 1;

                    char name[200]={ 0 };
                    strcpy(name, cmd.m_buff);
                    int name_len = strlen(name);
                    name[name_len -1] = '\0';

                    printf("puts_name =%s \n", name);

                    char hash_send[33] ={ 0 };
                    md5_file(name, result); 
                    printf("result = %s \n", result);

                    //while(1);
                    strcpy(hash_send, result); 
                    
                    printf("hash_send =%s \n", hash_send);
                    
                    send(newfd, hash_send, sizeof(hash_send), 0);


                    char puts_message[10] = { 0 };
                    recv(newfd, puts_message, sizeof(puts_message), 0);
                    //while(1);

                  if(strcmp(puts_message, "ok") == 0) 
                    cmd_puts(newfd, name);

                   // while(1);
                   
                  printf("puts 完成 \n");
                   
                   
                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);
                    
                    break;
                }
            case 7:
                {
                    cmd.m_cmd = CMD_TYPE_GETS;
                    //FD_CLR(clientfd, &rdset);
                    char name[200]={ 0 };
                    strcpy(name, cmd.m_buff);
                    int name_len = strlen(name);
                    name[name_len -1] = '\0';

                    printf("put_name =%s \n", name);
                   // char* hh = getcwd(NULL, 0);
                    // printf("this pwd is %s \n",  hh);
                    //strcpy(name,"/home/sunrise/桌面/wangdao/NetworkDisk/client/qwe.txt");

                   // close(clientfd);
                    cmd_gets(newfd, name);
                    
                    printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

                    break;
                }
            case 8:
                {
                    cmd.m_cmd = CMD_TYPE_QUIT;
                    quit = 1;
                    break;
                }
            default:
                printf(" recv error  cmd \n");
            }
            


           // recv(clientfd, buff, sizeof(buff), 0);
           // printf("%s @ %s : ", client.user, pwd);
           //      printf("\033[0m\033[1;32m%s@ubuntu\033[0m:\033[0m\033[1;34m%s\033[0m$  ", client.user, pwd);

            fflush(stdout);
              
        
        }
        if(quit == 1)
            break;
    }
    printf("\033[0m\033[1;31m bye!! \033[0m\n");

    close(clientfd);

    return 0;
}
