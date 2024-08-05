/*************************************************************************
    > File Name: ls.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 14时20分41秒
 ************************************************************************/

#ifndef  __LS_
#define  __LS_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"type.h"
#include <sys/types.h>
#include <dirent.h>
#include<assert.h>


char * cmd_ls(task_t result, char *root){
     
    
    char *s = (char *)malloc(sizeof(char) * 200);
    strcat(s,root);
    strcat(s,result.m_pwd);
   
    //打开目录流
    DIR *pdir = opendir(s);
    assert(pdir != NULL);
    long loc = telldir(pdir);
    struct dirent* pdirent;

    
    while(1){

        long tmploc = telldir(pdir);
        pdirent = readdir(pdir);

        if(pdirent == NULL){
            break;
        }else if( pdirent->d_type == DT_DIR){

            printf("\033[0m\033[1;34m%s\033[0m\n",pdirent->d_name);


        }else{
            printf("%s \n",pdirent->d_name);

        }


    }

    closedir(pdir);




    return s;
}

#endif
