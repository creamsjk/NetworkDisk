
client_main.c  是本文件的客户端文件 他的头文件是client.h
gcc client_main.c -o client -lcrypto

test_server.c 是测试客户端发送的文件 经过测试 目前写的client客户端问题不大,细节还需要和服务器对齐一点.
gcc test_server.c -o server 
