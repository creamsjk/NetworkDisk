/*************************************************************************
    > File Name: log_test.c
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月05日 星期一 20时02分35秒
    gcc log_test.c -o log_test
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<arpa/inet.h>
#include"log.h"

int main(void){

   log_t *log = (log_t*)malloc(sizeof(log_t));
   log_init(log);
   write_log(log,"你是谁");
   write_log(log,"我是秦始皇");
   write_log(log,"我不认识你");



    return 0;
}
