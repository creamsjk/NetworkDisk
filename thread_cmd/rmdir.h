/*************************************************************************
    > File Name: rmdir.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 15时26分46秒
 ************************************************************************/

#ifndef  __RMDIR__
#define  __MKDIR_


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"../type.h"
#include"../my_mysql/my_mysql.h"
#include<mysql/mysql.h>


int  cmd_rmdir(task_t result , char* root, MYSQL * pconn){

    char s[200]  = {0};
    int len = strlen(result.m_buff);
    result.m_buff[len -1] = '\0';
   // strcpy(s, root);
   // strcat(s, "/");
    char this_path[200] = {0};
    strcpy(this_path, result.m_pwd);
    int fir_ret = delete_file(pconn, result.m_buff, result.m_user, this_path); 
    strcat(s, result.m_pwd);
    strcat(s, "/");
    strcat(s,result.m_buff);
  

       int ret = rmdir(s);


       printf("rmdir ret =%d  |%s| \n", ret, s);
    return ret;

}




#endif 
