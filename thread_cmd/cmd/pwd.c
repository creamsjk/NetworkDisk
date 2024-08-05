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
#include"type.h"



char * cmd_pwd(task_t result, char *root){

    char * s = getcwd(NULL,0);
    int len  =  strspn(s,root);
    s += len-1;

    return s;

}


#endif
