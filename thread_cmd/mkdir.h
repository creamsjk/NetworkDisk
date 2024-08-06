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

int cmd_mkdir(task_t result, char *root){

    char *s = (char *)malloc(sizeof(char) * 200);
    strcpy(s, root);
    strcat(s, "/");
    strcat(s, result.m_pwd);
    strcat(s, "/");
    strcat(s,result.m_buff);
    // printf("%s \n",s);
    int ret = mkdir(s, 0777);
    free(s);
    
    return ret;
}





#endif
