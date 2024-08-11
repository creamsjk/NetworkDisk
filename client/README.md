
client_main.c  是本文件的客户端文件 他的头文件是client.h
gcc client_main.c -o client -lcrypto -w

test_server.c 是测试客户端发送的文件 经过测试 目前写的client客户端问题不大,细节还需要和服务器对齐一点.
gcc test_server.c -o server 

本项目客户端的文件全都存在本目录下面
puts 上传的也是本目录下面的文件
get  下载服务器文件到本目录下面

make 

./client  即可运行客户端
client_main.c 中修改端口 和ip 即可连接自己想要的服务器
