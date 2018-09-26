//

#ifndef _AE_EPOLL_HPP_
#define _AE_EPOLL_HPP_

#include "aeApi.hpp"
#include <sys/epoll.h>

typedef struct aeApiState {
    int epfd_;
    struct epoll_event *events_;
} aeApiState;

class AEEpoll : public AEApi
{
private:
    aeApiState *state_;
public:
    AEEpoll();
    ~AEEpoll();
    // 接口实现
    // 构造函数
    int aeApiCreate(aeEventLoop *eventLoop);
    // 析构函数
    void aeApiFree(aeEventLoop *eventLoop);
    int aeApiResize(aeEventLoop *eventLoop, int setsize);
    // 添加事件
    int aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask);
    // 删除事件
    void aeApiDelEvent(aeEventLoop *eventLoop, int fd, int delmask);
    // 事件就绪检查
    int aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp);
    char *aeApiName(void);
};

#endif