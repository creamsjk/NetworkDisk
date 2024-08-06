/*************************************************************************
 char result[33] = {0};    > File Name: md5.h
    > Author: sunrie
    > Mail: 1102490208@qq.com 
    > Created Time: 2024年08月06日 星期二 09时37分36秒
 ************************************************************************/

#ifndef  __MD5_
#define  __MD5_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/md5.h>

void md5hexToString(unsigned char *md,char *result){
    // char tmp[3];
    for (size_t i = 0; i <= 15; i++){
        sprintf(result+i*2,"%02x",md[i]);
    }
    return;
}

void md5_to(const unsigned char *s, size_t len , unsigned char* md, char *result){

    MD5(s, len, md);
    md5hexToString(md, result);  

}



#endif
