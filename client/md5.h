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

void md5_file(const char *filename,  char * result){
    
    FILE *file = fopen(filename, "rb");
    if(file == NULL){
        perror("Errpr opening file ");
        exit(1);
    }
    MD5_CTX context;

    MD5_Init(&context);

    unsigned char buffer[1024];

    size_t len;

    while((len = fread(buffer, 1, sizeof(buffer), file)) > 0){

        MD5_Update(&context, buffer, len);
    }
    unsigned char md[16] = { 0 };

    MD5_Final(md, &context);
    memset(result, '\0', sizeof(result));
    
    for(unsigned int i =0 ;i <16 ;i++){
        char frag[3] = {0};
        sprintf(frag, "%02x", md[i]);
        strcat(result, frag);

    }
   //成功完成 

    fclose(file);
}



#endif
