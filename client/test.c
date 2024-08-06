/*************************************************************************
    > File Name: test.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月06日 星期二 09时45分17秒
    gcc md5.c -lcrypto -o md5
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include"md5.h"
#include"client.h"

int main(void){



      cmd_type cmd;
      cmd = CMD_TYPE_PWD;
      printf("cmd == %d \n",cmd);

     unsigned char md[16] = {0};
     char result[32] = {0};

     char password[200]= {0};
     strcpy(result, "pwd          nihaowoshi\0");
   //  md5_to((unsigned char *)password, sizeof(password), md, result);


              char * send_cmd;
              int i = 0;

                  while(result[i] == ' ')
                      i++;
              
              send_cmd = result + i;
              printf("%s  \n",send_cmd);

              while(result[i] != '\0' &&  result[i++] != ' ')
                   ;
              if(result[i] != '\0')
                 result[i-1]='\0';

              while(result[i] == ' ')
                   i++;
             

             char *send_buff =  result + i;

     printf("send_cmd ==%s   send_buff ==%s \n", send_cmd, send_buff);

     int c =  transfer_cmd(send_cmd);
     printf("cmd ==%d \n",c);

    // printf("result  %s \n", result);

     return 0;
}
