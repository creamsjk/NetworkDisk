/*************************************************************************
    > File Name: cmd_test.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 13时46分04秒

         gcc   cmd_test.c  -o cmd_test

 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"pwd.h"
#include"ls.h"
#include"cd.h"
#include "mkdir.h"
#include"rmdir.h"

int main(void){

    char * root = "/home/sunrise/桌面/wangdao";

   task_t r;
   r.m_cmd = CMD_TYPE_PWD;
   r.m_peerfd = 0;
   strcpy(r.m_buff,"qqq");
   strcpy(r.m_pwd,"/NetworkDisk/sjk");

   char *h = cmd_pwd(r, root);
   //char *h = cmd_ls(r, root);
   //char *h = cmd_cd(r,root);
   //int ret  = cmd_mkdir(r, root);   
   // int ret  = cmd_rmdir(r, root);

   printf("%s \n",h);


   return 0;

    
}
