/*************************************************************************
    > File Name: cd.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 15时05分19秒

    目前完全成功!! 可以正常 cd ..  cd .
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
#include"../my_mysql/my_mysql.h"
#include<mysql/mysql.h>

char *cmd_cd(task_t result, char *root, MYSQL *pconn){
    
    char *s = (char*)malloc(sizeof(char) * 200);
    memset(s,'\0', 200);
    strcat(s, result.m_pwd);
    strcat(s, "/");
    strcat(s,result.m_buff);
    
    //strcat(s, result.m_buff);
   // strcpy(s,"/home/sunrise/桌面/wangdao/NetworkDisk/home/dir_a");
    //字符串s最后面有一个\n
    
    
   
    int d=strlen(s);
    //printf("slen =%d \n",d);
    s[d-1]='\0';

   char * ff = strrchr(s, '/');
   char dir_name[200] = { 0 };
   char this_path[200] = { 0 };

   strcpy(this_path, result.m_pwd);
   strcpy(dir_name, ff + 1);
   int ret = find_user_file_is_exist(pconn, result.m_user, dir_name, this_path);



   

    //  printf("ff ==%s \n", ff + 1);
    //   *ff = '\0';
    //   printf("s ==%s \n", s);
   //    printf("m_pwd =%s \n",result.m_pwd);
    
   if(strcmp(ff + 1, "..") == 0){

       // printf("ff ==%s \n", ff + 1);
       *ff = '\0';
       ff = strrchr(s, '/');

       if(strcmp(ff, "/home") != 0)
       *ff = '\0';
       // printf("s ==%s \n", s);
   }else if(strcmp(ff + 1, ".") == 0){

       *ff = '\0';
   }else if (ret != 1){

       *ff = '\0';
       
   }



    struct stat st;
     ret = stat(s, &st);
   
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
