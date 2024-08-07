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
#include <sys/types.h>
#include <sys/stat.h>
#include<dirent.h>
#include"../type.h"

char *cmd_cd(task_t result, char *root){
    
    char *s = (char*)malloc(sizeof(char) * 200);
    memset(s,'\0', 200);
    strcat(s, result.m_pwd);
    strcat(s, "/");
    strcat(s,result.m_buff);
    
    //strcat(s, result.m_buff);
   // strcpy(s,"/home/sunrise/桌面/wangdao/NetworkDisk/home/dir_a");
    //字符串s最后面有一个\n
   
    int d=strlen(s);
    printf("slen =%d \n",d);
    s[d-1]='\0';
    struct stat st;
    int ret = stat(s, &st);
   
    //printf("cd_ =%sret = %d \n", s, ret);
    //如果不是就返回错误信息
    if(!S_ISDIR(st.st_mode)){
        char *h  = (char*)malloc(sizeof(char) * 32);
        strcpy(h,"error\0");
        return h;
    }

    //printf("aaaa \n");
    int len = strspn(root, s);
    s += len;

    //printf("%s \n",s);
    
    return s;
}



#endif
