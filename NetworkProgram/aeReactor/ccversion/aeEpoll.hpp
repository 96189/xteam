//

#ifndef _AE_EPOLL_HPP_
#define _AE_EPOLL_HPP_

#include "aeApi.hpp"
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

class aeApiState 
{
public:
    int epfd_;
    struct epoll_event *events_;
public:
    aeApiState(int size)
    {
        events_ = (struct epoll_event *)malloc(sizeof(struct epoll_event) * size);
        assert(events_ != NULL);
        epfd_ = epoll_create(1024); /* 1024 is just a hint for the kernel */
        assert(epfd_ != -1);
    }
    ~aeApiState()
    {
        close(epfd_);
        free(events_);
    }
};

class aeEventLoop;
class AEEpoll : public AEApi
{
private:
    aeApiState *state_;
    aeEventLoop *eventLoop_;
public:
    AEEpoll(aeEventLoop *eventLoop);
    ~AEEpoll();
    // 接口实现
    // 构造函数
    int aeApiCreate();
    // 析构函数
    void aeApiFree();
    int aeApiResize(int setsize);
    // 添加事件
    int aeApiAddEvent(int fd, int mask);
    // 删除事件
    void aeApiDelEvent(int fd, int delmask);
    // 事件就绪检查
    int aeApiPoll(struct timeval *tvp);
    char *aeApiName(void);
};

#endif