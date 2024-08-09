/*************************************************************************
    > File Name: mkdir.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 15时13分42秒
 ************************************************************************/
#ifndef __MKDIR_
#define __MKDIR_


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"../type.h"
#include<mysql/mysql.h>
#include"../my_mysql/my_mysql.h"

int cmd_mkdir(task_t result, char *root,MYSQL *pconn ){

    char s[200] = { 0 };
    int len = strlen(result.m_buff);
    result.m_buff[len -1 ] = '\0';
    //strcpy(s, root);
    //strcat(s, "/");
    
    //s = strpbrk(result.m_buff, "/");
    //printf("mkdir m_pwd=%s| \n",result.m_pwd);
    strcat(s, result.m_pwd);
    strcat(s, "/");
    strcat(s,result.m_buff);
     //printf(" mkdir =%s \n",s);
     //char * begin = s;
     //printf("s[0] = %s \n",begin + 2);

    int ret = mkdir(s, 0771);
    //int ret = mkdir("/home/sunrise/桌面/wangdao/NetworkDisk/home/dir_a", 0771);
    if(ret == 0){
        //目录的hash是 用户名字  大小是4096
        insert_file(pconn, result.m_buff, "d", result.m_user, result.m_user , result.m_pwd);
        insert_global_file(pconn, result.m_user, result.m_buff,  4096);


    }
    //printf("%d \n",ret);
    
    return ret;
}





#endif
