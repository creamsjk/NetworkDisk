#include "my_mysql.h"

MYSQL* connect_db()
{
    MYSQL* pconn = mysql_init(NULL);
    if(pconn == NULL)
    {
        perror("mysql_init() failed.");
        return NULL;
    }
    
    const char* host = "118.25.193.224";
    const char* user = "root";
    const char* passwd = "104962dec5f28b2e";
    const char* db = "Cloudisk";

    if(mysql_real_connect(pconn, host, user, passwd, db, 0, NULL, 0) == NULL)
    {
        perror(mysql_error(pconn));
        mysql_close(pconn);    
        return NULL;
    }
    printf("connect mysql successfully.\n");

    return pconn;
}

int execute_query(MYSQL* pconn, const char* sql)
{
    if(mysql_real_query(pconn, sql, strlen(sql)) != 0)
    {
        perror(mysql_error(pconn));
        return -1;
    }
    return 0;
}

/*
 * 用户名是否已经存在
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 用户名
 * 返回值：用户存在返回1
 *         用户不存在返回0
 *         失败返回-1
 * */
int find_user_is_exist(MYSQL* pconn, char* username)
{
    char sql[256];
    sprintf(sql, "select username from User where username = '%s';", username);
    
    if(execute_query(pconn, sql) == -1)
        return -1;
    
    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int rows = mysql_num_rows(pres);
        mysql_free_result(pres);   
        if(rows > 0)
            return 1; //用户存在 
        else
            return 0; //用户不存在
    }
    else
    {
        perror(mysql_error(pconn));
        return -1;
    }    
}

/*
 * 添加新用户
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 用户名
 *       password 用户密文
 *       pwd 用户当前工作目录
 * 返回值：添加成功返回0
 *         失败返回-1
 * */
int insert_user(MYSQL* pconn, char* username, char* password, char* pwd)
{
    printf("usser->name = %s, %ld\n", username, strlen(username));
    printf("user->password = %s, %ld\n", password, strlen(password));
    printf("user->pwd = %s, %ld\n", pwd, strlen(pwd));
    char sql[256];
    sprintf(sql, "insert into User values('%s', '%s', '%s');", username, password, pwd);

    if(execute_query(pconn, sql) == -1)
        return -1;

    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);

    return 0;
}

/*
 * 通过用户名获取用户密码密文
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 需要查找的用户名
 * 返回值：成功返回密文
 *         失败返回NULL
 * */
char* get_user_password(MYSQL* pconn, char* username)
{
    char sql[256];
    sprintf(sql, "select password from User where username = '%s';", username);

    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        MYSQL_ROW row = NULL;
        char* result = NULL;
        if((row = mysql_fetch_row(pres)) != NULL)
        {
            result = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result, row[0]);
        }

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

/*
 * 通过用户名获取用户当前工作路径
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 需要查找的用户名
 * 返回值：成功返回当前工作路径
 *         失败返回NULL
 * */
char* get_user_pwd(MYSQL* pconn, char* username)
{
    char sql[256];
    sprintf(sql, "select pwd from User where username = '%s';", username);

    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        MYSQL_ROW row = NULL;
        char* result = NULL;
        if((row = mysql_fetch_row(pres)) != NULL)
        {
            result = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result, row[0]);
        }

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

/*
 * 更新用户当前工作目录
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 用户名
 *       new_pwd 用户新的当前工作目录
 * 返回值：成功返回0
 *         失败返回-1
 * */
int update_user_pwd(MYSQL* pconn, char* username, char* new_pwd)
{
    char sql[256];
    sprintf(sql, "update User set pwd = '%s' where username = '%s';",new_pwd, username);

    if(execute_query(pconn, sql) == -1)
        return -1;

    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);

    return 0;
}

/*
 * 查找用户目录是否存在
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 特定的用户名
 *       filename 需要查找的目录名
 *       filepath 在特定路径下查找
 * 返回值：文件名存在返回1
 *         文件名不存在返回0
 *         失败返回-1
 * */
int find_user_file_is_exist(MYSQL* pconn, char* username, char* filename, char* filepath)
{
    char sql[256];
    sprintf(sql, "select filename from File where username = '%s' and filename = '%s' and filepath = '%s' and filetype = 'd';", username, filename, filepath);

    if(execute_query(pconn, sql) == -1)
        return -1;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int rows = mysql_num_rows(pres);
        mysql_free_result(pres);
        if(rows > 0)
            return 1;
        else 
            return 0;
    }
    else
    {
        perror(mysql_error(pconn));
        return -1;
    }
}

/*
 * 查找文件是否已经在服务器上
 * 参数：pconn 指向MYSQL结构体的指针
 *       file_hash 确定文件唯一的密文
 * 返回值：文件名存在返回1
 *         文件名不存在返回0
 *         失败返回-1
 * */
int find_file_is_exist(MYSQL* pconn, char* file_hash)
{
    char sql[256];
    sprintf(sql, "select filename from GlobalFile where file_hash = '%s';", file_hash);
    
    if(execute_query(pconn, sql) == -1)
        return -1;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int rows = mysql_num_rows(pres);
        mysql_free_result(pres);
        if(rows > 0)
            return 1;
        else
            return 0;
    }
    else
    {
        perror(mysql_error(pconn));
        return -1;
    }
}

/*
 * 通过用户名获取该用户所有文件名
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 要查找的用户名
 * 返回值：获取成功返回文件名的字符串数组
 *         失败或者没有记录返回NULL
 * */
char** get_user_file(MYSQL* pconn, char* username)
{
    char sql[256];
    sprintf(sql, "select filename from File where username = '%s';", username);
    
    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int rows = mysql_num_rows(pres);
        int cols = mysql_num_fields(pres);
        int row_count = 0;

        char** result = (char**)malloc(sizeof(char*) * (rows + 1));
        MYSQL_ROW row = NULL;
        while((row = mysql_fetch_row(pres)) != NULL)
        {
            result[row_count] = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result[row_count++], row[0]);
        }
        result[row_count] = NULL;

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

/*
 * 获取路径下的用户文件
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 要查找的用户名
 *       path 查找路径
 * 返回值：获取成功返回文件名的字符串数组
 *         失败或者没有记录返回NULL
 * */
char** get_user_file_by_path(MYSQL* pconn, char* username, char* path)
{
    char sql[256];
    sprintf(sql, "select filename from File where username = '%s' and filepath = '%s' and filetype = 'f';", username, path);

    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int rows = mysql_num_rows(pres);
        int cols = mysql_num_fields(pres);
        int row_count = 0;

        char** result = (char**)malloc(sizeof(char*) * (rows + 1));
        MYSQL_ROW row = NULL;
        while((row = mysql_fetch_row(pres)) != NULL)
        {
            result[row_count] = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result[row_count++], row[0]);
        }
        result[row_count] = NULL;

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

/*
 * 获取路径下的用户文件
 * 参数：pconn 指向MYSQL结构体的指针
 *       username 要查找的用户名
 *       path 查找路径
 * 返回值：获取成功返回文件名的字符串数组
 *         失败或者没有记录返回NULL
 * */
char** get_user_dir_by_path(MYSQL* pconn, char* username, char* path)
{
    char sql[256];
    sprintf(sql, "select filename from File where username = '%s' and filepath = '%s' and filetype = 'd';", username, path);

    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int rows = mysql_num_rows(pres);
        int cols = mysql_num_fields(pres);
        int row_count = 0;

        char** result = (char**)malloc(sizeof(char*) * (rows + 1));
        MYSQL_ROW row = NULL;
        while((row = mysql_fetch_row(pres)) != NULL)
        {
            result[row_count] = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result[row_count++], row[0]);
        }
        result[row_count] = NULL;

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

/*
 * 获取文件的大小
 * 参数：pconn 指向MYSQL结构体的指针
 *       file_hash 文件的唯一标识
 * 返回值：查找成功返回文件大小
 *         失败返回-1
 * */
size_t get_file_size(MYSQL* pconn, char* file_hash)
{
    char sql[256];
    sprintf(sql, "select filesize from GlobalFile where file_hash = '%s';", file_hash);

    if(execute_query(pconn, sql) == -1)
        return -1;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        size_t result = -1;
        MYSQL_ROW row = NULL;

        if((row = mysql_fetch_row(pres)) != NULL)
            result = atoi(row[0]);

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return -1;
    }
}

/*
 * 获取文件的hash值
 * 参数：pconn 指向MYSQL结构体的指针
 *       filename 需要查找的文件名
 *       username 所属的用户名
 * 返回值：查找成功返回文件的哈希值
 *         失败返回NULL
 * */
char* get_file_hash(MYSQL* pconn, char* filename, char* username)
{
    char sql[256];
    sprintf(sql, "select file_hash from File where filename = '%s' and username = '%s';", filename, username);

    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        MYSQL_ROW row = NULL;
        char* result = NULL;
        if((row = mysql_fetch_row(pres)) != NULL)
        {
            result = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result, row[0]);
        }

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

/*
 * 获取用户文件路径
 * 参数：pconn 指向MYSQL结构体的指针
 *       filename 需要查找的文件名
 *       username 指定的用户
 * 返回值：查找成功返回文件路径
 *         失败返回NULL
 * */
char* get_filepath(MYSQL* pconn, char* filename, char* username)
{
    char sql[256];
    sprintf(sql, "select filepath from File where filename = '%s' and username = '%s';", filename, username);

    if(execute_query(pconn, sql) == -1)
        return NULL;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        MYSQL_ROW row = NULL;
        char* result = NULL;
        if((row = mysql_fetch_row(pres)) != NULL)
        {
            result = (char*)malloc(sizeof(char) * (strlen(row[0]) + 1));
            strcpy(result, row[0]);
        }

        mysql_free_result(pres);
        return result;
    }
    else
    {
        perror(mysql_error(pconn));
        return NULL;
    }
}

int get_user_count(MYSQL* pconn, char* file_hash);
static int update_global_file_usercount(MYSQL* pconn, char* file_hash, int count);
/*
 * 用户添加新文件
 * 参数：pconn 指向MYSQL结构体的指针
 *       filename 文件名
 *       filetype 文件类型
 *       username 用户名
 *       filepath 文件路径
 * 返回值：添加成功返回0
 *         失败返回-1
 * */
int insert_file(MYSQL* pconn, char* filename, char* filetype, char* username, char* file_hash, char* filepath)
{
    char sql[256];
    sprintf(sql, "insert into File(filename, filetype, username, file_hash, filepath) values('%s', '%s', '%s', '%s', '%s');", filename, filetype, username, file_hash, filepath);

    if(execute_query(pconn, sql) == -1)
        return -1;
    
    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);
    
    int ret = -1;
    if(find_file_is_exist(pconn, file_hash) == 1)
    {
        int usercount = get_user_count(pconn, file_hash);
        if(usercount == -1)
            return -1;
        ret = update_global_file_usercount(pconn, file_hash, usercount + 1);
        if(ret == -1)
            return -1;
    }

    return 0;
}

/*
 * 添加了服务器上没有的文件
 * 参数：pconn 指向MYSQL结构体的指针 
 *       file_hash 文件密文
 *       filename 文件名
 *       filesize 已上传的文件大小
 * 返回值：添加成功返回0
 *         失败返回-1
 * */
int insert_global_file(MYSQL* pconn, char* file_hash, char* filename, size_t filesize)
{
    char sql[256];
    sprintf(sql, "insert into GlobalFile(file_hash, filename, filesize) values('%s', '%s', %ld);", file_hash, filename, filesize);

    if(execute_query(pconn, sql) == -1)
        return -1;

    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);

    return 0;
}

int get_user_count(MYSQL* pconn, char* file_hash)
{
    char sql[256];
    sprintf(sql, "select usercount from GlobalFile where file_hash = '%s';", file_hash);

    if(execute_query(pconn, sql) == -1)
        return -1;

    MYSQL_RES* pres = mysql_store_result(pconn);
    if(pres)
    {
        int usercount = -1;
        MYSQL_ROW row = NULL;
        if((row = mysql_fetch_row(pres)) != NULL)
        {
            usercount = atoi(row[0]);
            printf("usercount = %d\n", usercount);
        }

        mysql_free_result(pres);
        return usercount;
    }
    else
    {
        perror(mysql_error(pconn));
        return -1;
    }
}

static int delete_user_file(MYSQL* pconn, char* filename, char* username)
{
    char sql[256];
    sprintf(sql, "delete from File where filename = '%s' and username = '%s';", filename, username);

    if(execute_query(pconn, sql) == -1)
        return -1;

    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);

    return 0;
}

static int delete_global_file(MYSQL* pconn, char* file_hash)
{
    char sql[256];
    sprintf(sql, "delete from GlobalFile where usercount = 1 and file_hash = '%s';", file_hash);

    if(execute_query(pconn, sql) == -1)
        return -1;

    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);

    return 0;
}

static int update_global_file_usercount(MYSQL* pconn, char* file_hash, int count)
{
    char sql[256];
    sprintf(sql, "update GlobalFile set usercount = %d where file_hash = '%s';", count, file_hash);

    if(execute_query(pconn, sql) == -1)
        return -1;

    int rows = mysql_affected_rows(pconn);
    printf("QUERY OK, %d row affected.\n", rows);

    return 0;
}

/*
 * 删除文件
 * 参数：
 *  pconn 指向MYSQL结构体的指针
 *  filename 需要删除的文件名
 *  username 特定的用户名
 * 返回值：
 *  成功0
 *  失败-1
 * */
int delete_file(MYSQL* pconn, char* filename, char* username)
{
    char* file_hash = get_file_hash(pconn, filename, username);
    int usercount = get_user_count(pconn, file_hash);
    int ret;
    if(usercount == 1)
    {
        ret = delete_user_file(pconn, filename, username);
        if(ret == -1)
            return -1;
        ret = delete_global_file(pconn, file_hash);
        if(ret == -1)
            return -1;
    }
    else
    {
        usercount--;
        ret = delete_user_file(pconn, filename, username);
        if(ret == -1)
            return -1;
        ret = update_global_file_usercount(pconn, file_hash, usercount);
        if(ret == -1)
            return -1;
        
    }

    return 0;
}

void close_db(MYSQL* pconn)
{
    if(pconn)
        mysql_close(pconn);
}
