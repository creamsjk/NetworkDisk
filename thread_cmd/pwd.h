/*************************************************************************
    > File Name: cmd.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 13时38分33秒
 ************************************************************************/

#ifndef  __PWD
#define  __PWD

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include"../type.h"



char * cmd_pwd(task_t result, char *root){

    int ret = chdir(result.m_pwd);
    if(ret == -1){
        char s[20];
        strcpy(s, "pwd failed !!");
        return s;
    }
    char * s = getcwd(NULL,0);
   // printf("cmd_pwd s ==%s   ",s);
   // int len  =  strspn(s,root);
    int len = strspn(root, s);
    s += len;

    return s;

}


#endif
