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
#include"../type.h"
#include <sys/types.h>
#include <dirent.h>
#include<assert.h>
#include<mysql/mysql.h>
#include"../my_mysql/my_mysql.h"


char * cmd_ls(task_t result, char *root, MYSQL * pconn){
     
    
   // printf("ls_pwd = %s \n",result.m_pwd);
    char *s = (char *)malloc(sizeof(char) * 200);
    memset(s,'\0', sizeof(s));
    //strcat(s,root);
    strcat(s,result.m_pwd);
    
    char * message = (char *)malloc(sizeof(char) * 10240);
    memset(message, '\0', 10240);
     printf("dir == %s  m_pwd == %s \n", s, result.m_pwd);

     //目前只查询了 普通文件
     char ** sql_result = get_user_file_by_path(pconn, result.m_user, s); 

    int i = 0;
    while(sql_result[i] != NULL){
        strcat(message, sql_result[i]);
        strcat(message, "  ");
        i++;
    }
    free(sql_result);
     
   //printf("%s \n", message);
   
   //查询目录文件 但是不传出去
   
    sql_result = NULL;
    sql_result = get_user_dir_by_path(pconn, result.m_user, s);
    
    i = 0;
     while(sql_result[i] != NULL){
        strcat(message, sql_result[i]);
        strcat(message, "  ");
        i++;
    }
    free(sql_result);
       

   
//    //打开目录流
//    DIR *pdir = opendir(s);
//    //assert(pdir != NULL);
//    if(pdir == NULL){
//      strcpy(message, "no this DIR !!!");
//      return message;
//    }
//    long loc = telldir(pdir);
//    struct dirent* pdirent;
//
//    
//    while(1){
//
//        long tmploc = telldir(pdir);
//        pdirent = readdir(pdir);
//
//        if(pdirent == NULL){
//            break;
//        }else if( pdirent->d_type == DT_DIR){
//
//            //printf("\033[0m\033[1;34m%s\033[0m\n",pdirent->d_name);
//            strcat(message, pdirent->d_name);
//            strcat(message, " ");
//
//
//        }else{
//            //printf("%s \n",pdirent->d_name);
//            strcat(message, pdirent->d_name);
//            strcat(message, " ");
//            
//
//        }
//
//
//    }
//
//    closedir(pdir);
//

//初步完成  全部地下文件一起 发送的 
  
     //使用数据库查询完成
     

    return message;
}

#endif
