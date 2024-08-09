#include <func.h>
#include "my_mysql.h"
#include "user.h"

int main(int argc, char* argv[])
{
    user_t user;
    user_t user2;
    memset(&user, 0, sizeof(user));
    strcpy(user.username, "ccxts");
    strcpy(user.password, "JNBHJASDKNJNKAFD5S8ERAU7S8UJ18SJ");

    MYSQL* pconn = connect_db();

   /* 
    int ret = insert_user(pconn, user);
    if(ret == 0)
        printf("insert_user successfully.\n");
    */

    /*
    int ret = get_user_info(pconn, "ccxts", &user2);
    if(ret == 0)
    {
        printf("get_user_info() successfully.\n");
        printf("username = %s\n", user2.username);
        printf("password = %s\n", user2.password);
    }
    */
    /*
    int ret = find_user_is_exist(pconn, &user);
    if(ret == 1)
        printf("user is exist\n");

    return 0;
    */

    /*
    char** user_files = get_user_file(pconn, user);
    if(user_files == NULL)
    {
        printf("get_user_file() failed.\n");
        return -1;
    }
    for(int i = 0; user_files[i] != NULL; i++)
    {
        printf("user_files[%d] = %s\n", i, user_files[i]);
        free(user_files[i]);
    }
    free(user_files);
    */
    /*
    size_t filesize = get_file_size(pconn, "text2");
    printf("filesize = %ld\n", filesize);

    char* file_hash = get_file_hash(pconn, "text3");
    printf("file_hash = %s\n", file_hash);

    char* file_path = get_filepath(pconn, "text2", user.username);
    printf("file_path = %s\n", file_path);
    */
    /*
    int ret = delete_file(pconn, "text1", "ccxts"); 
    if(ret == -1)
        printf("failed.\n");
    */

    insert_file(pconn, "text2", "f", "ccxwz", "SHBGJASDKNJNKAFD5S8ERAU7S8UJ18SJ", "/ccxwz");

    return 0;
}

