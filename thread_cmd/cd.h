/*************************************************************************
    > File Name: cd.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 15时05分19秒
 ************************************************************************/

#ifndef  __CD_
#define  __CD_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"../type.h"

char *cmd_cd(task_t result, char *root){
    
    char *s = (char*)malloc(sizeof(char) * 200);
    strcat(s, root);
    strcat(s, "/");
    strcat(s, result.m_buff);

    printf("%s \n",s);
    return s;
}



#endif
