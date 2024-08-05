#include "server.h"

typedef enum cmd_t{
    CMD_TYPE_PWD = 1,
    CMD_TYPE_LS,
    CMD_TYPE_CD,
    CMD_TYPE_MKDIR,
    CMD_TYPE_RMDIR,
    CMD_TYPE_PUTS,
    CMD_TYPE_GETS,
    CMD_TYPE_QUIT,
    CMD_TYPE_NOTCMD


}cmd_type;

int main(void)
{
    printf("enum size: %d\n", sizeof(cmd_type));    
    return 0;
}

