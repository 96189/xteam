#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include <uuid/uuid.h>
#include <thread>
#include "net.h"
#include "chatMessage.pb.h"

typedef struct
{
    char ip[64];
    int port;
    bool echo;
    bool chat;
    bool transfile;
} Option;
#define BUFFER_SIZE (1024*1024)
Option g_option;

bool ParseCommandLine(int argc, char *argv[], const char *optString, Option *option)
{
    int opt = -1;
    int ret_code = false;
    printf("Start Server at\n");
    while ((opt = getopt(argc, argv, optString)) != -1)
    {
        ret_code = true;
        switch (opt)
        {
        // ip
        case 'h':
            strcpy(option->ip, optarg);
            printf("ip:%s\n", option->ip);
            break;
        // port
        case 'p':
            option->port = atoi(optarg);
            printf("port:%d\n", option->port);
            break;
        default:
            ret_code = false;
            break;
        }
    }
    return ret_code;
}
// 生成uuid(待优化)
int GenerateUid()
{
    char uid[128] = {};
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid,uid);
    return (atoi(uid));
}
// 根据uid和content填充pkg
void SetPkg(app::UserPackage& pkg, const int uid, const std::string& content)
{
    pkg.mutable_head()->set_id(uid);
    pkg.mutable_body()->set_content(content.c_str());
}
// 
void Encode(std::string& message, app::UserPackage& pkg, const std::string& content)
{
    // 序列化
    pkg.SerializeToString(&message);
}
void Decode(app::UserPackage& pkg, const std::string& message)
{
    // 反序列化
    pkg.ParseFromString(message);
}

void ReadSocket(int connfd)
{
    char buffer[BUFFER_SIZE];
    int count = 0;
    app::UserPackage recv_pkg;
    std::string recv_message;
    while ((count = recv(connfd, buffer, BUFFER_SIZE, 0)) > 0)
    {
        recv_pkg.Clear();
        recv_message.clear();
        recv_message.assign(buffer, count);
        Decode(recv_pkg, recv_message);
        
        printf("recv:%d %d %s\n", count, recv_pkg.head().id(), recv_pkg.body().content().c_str());
    }
    printf("ReadSocket exit\n");
    ::exit(0);
}
// 读socket->写stdout   one thread
// 读stdin->写socket    one thread
void Run(int connfd)
{
    // 读socket 写stdout thread
    std::thread thr(ReadSocket, std::move(connfd));
    
    // 读stdin 写socket thread(主线程)
    char buffer[BUFFER_SIZE];
    int count = 0;
    app::UserPackage pkg;
    std::string message;
    std::string content;
    int uid = GenerateUid();
    while ((count = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
    {
        pkg.Clear();
        content.clear();
        message.clear();

        content.assign(buffer, count);
        SetPkg(pkg, uid, content);
        Encode(message, pkg, content);
        int nw = send(connfd, message.c_str(), message.size(), 0);
        if (nw < count)
        {
            break;
        }
    }
    printf("Run exit\n");
    // 资源回收
    close(connfd);    
    // 线程回收
    thr.join();
}

int main(int argc, char *argv[])
{
    const char *optString = "h:p:";
    memset(&g_option, 0, sizeof(g_option));
    int ret = ParseCommandLine(argc, argv, optString, &g_option);
    assert(ret);

    int sockfd;
    ret = CNET::tcp_connect(&sockfd, g_option.ip, g_option.port, 1);
    assert(ret != -1);

    Run(sockfd);
    return 0;
}