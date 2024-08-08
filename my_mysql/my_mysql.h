#ifndef MY_MYSQL_H
#define MY_MYSQL_H

#include<stdio.h>
#include<string.h>
#include <mysql/mysql.h>

MYSQL* connect_db(void);
int execute_query(MYSQL* , const char* );
void close_db(MYSQL* );

int find_user_is_exist(MYSQL* , char* ); //检查用户名是否存在
int insert_user(MYSQL* , char* , char* , char* ); //新增用户
char* get_user_password(MYSQL* , char* ); //获取用户密文
char* get_user_pwd(MYSQL* , char* ); //获取用户当前工作路径
int update_user_pwd(MYSQL* , char* , char* ); //更新用户当前工作路径

int find_file_is_exist(MYSQL* , char* ); //检查文件名是否存在
char** get_user_file(MYSQL* , char* ); //获取所有特定用户的文件
char** get_user_file_by_path(MYSQL* , char* , char* ); //获取路径下的用户文件
char** get_user_dir_by_path(MYSQL* , char* , char* );//获取路径下的用户目录文件
size_t get_file_size(MYSQL* , char* ); //获取文件大小
char* get_file_hash(MYSQL* , char* ); //获取文件hash值
char* get_filepath(MYSQL* , char* , char* ); //获取文件路径
int insert_file(MYSQL* , char* , char* , char* , char* ,char* ); //用户添加新文件
int insert_global_file(MYSQL* , char* , char* , size_t ); //添加服务器上没有的文件

#endif

