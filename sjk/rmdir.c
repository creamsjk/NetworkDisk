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
#include"type.h"


int  cmd_rmdir(task_t result , char* root){

    char *s = (char *)malloc(sizeof(char) * 200);
    strcpy(s, root);
    strcat(s, "/");
    strcat(s, result.m_pwd);
    strcat(s, "/");
    strcat(s,result.m_buff);

    int ret = rmdir(s);
    
    free(s);
    return ret;

}




#endif 
