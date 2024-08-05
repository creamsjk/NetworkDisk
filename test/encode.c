#include <shadow.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//返回盐值地址
char* userLoginCheck1(const char* username){
    //通过用户名获取用户salt值
    struct spwd* pSpwd = (struct spwd*) calloc(1, sizeof(struct spwd));
    pSpwd = getspnam(username);
    //printf("encrypted password: %s\n", pSpwd->sp_pwdp);
    
    char buf[256] = { 0 };
    struct spwd *temp = pSpwd;
    for(int i = 0, j = 0; temp->sp_pwdp[i]; i++){
        if(temp->sp_pwdp[i] == '$'){
            j++;
        }
        //第三个$之后
        if(j == 3){
            int k = i + 1;
            while(temp->sp_pwdp[k] != ':'){
                k++;
            }
            strncpy(buf, temp->sp_pwdp + i + 1, k - i - 1);
            break;
        } 
    }

    //printf("salt: %s\n", buf);
    return buf;
}



int main(int argc, char* argv[])
{
    userLoginCheck1(argv[1]);
    return 0;
}

