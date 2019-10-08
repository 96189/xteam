#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#include "TimerManager.h"
#include <string>
#include <vector>
#include <errno.h>
#include "net.h"

#define MAX_EVENT_NUM 1024
#define TIMEOUT 60

typedef struct
{
    char ip[64];
    int port;
    bool echo;
    bool chat;
    bool transfile;
} Option;

// epoll_create() epfd
extern int g_epollfd;
// listen() listen_fd
extern int g_listenfd;
// connfd -> conn_data 映射表
extern ClientMap g_users_map;
// 全局定时器管理对象
extern TimerManager g_timer_manager;
// 命令行参数对象
extern Option g_option;

int split_str(const char *ps_str, char *ps_sp, std::vector<std::string> &v_ret);
int setnonblocking(int fd);

// epoll事件修改
void EnableInput(uint32_t &events);
void EnableOutput(uint32_t &events);
void DisableInput(uint32_t &events);
void DisableOutput(uint32_t &events);

// 连接定时器超时回调,默认行为是断开连接
void cb_func(int fd);
// 统一IO时间和定时器事件
int UnifyEvent(int g_epollfd, epoll_event *events, int max_event_num);
// epoll_ctl add del modify
void UpdateEvents(int epfd, int fd, int event, int op);
// 设置栈缓冲区的数据到conn的缓冲区中
void SetConnData2Cache(int conn, char *buffer, int len);
// 发送conn的缓冲区的数据
void SendCacheData2Conn(int conn);
// 清空连接缓冲区
void ResetConnCache(int conn);

// 处理新连接建立
void ProcAccept(int listenfd);
// 处理已建立的连接消息到达
void ProcRead(int sockfd);
// 处理可读事件
void HandleInput(int sockfd, uint32_t events);
// 处理可写事件
void HandleOutput(int sockfd, uint32_t events);
// echo业务逻辑
void Echo(int sockfd, char *buffer, int len);
// Chat业务逻辑
void Chat(int sockfd, char *buffer, int len);
// TransFile业务逻辑
void TransFile(int sockfd, char *buffer);
void SendFile(int connfd, std::string filename);
#endif