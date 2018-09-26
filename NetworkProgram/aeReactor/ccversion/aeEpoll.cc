#include "aeEpoll.hpp"
#include "aeEventLoop.hpp"

// 构造 析构不做实际工作
AEEpoll::AEEpoll()
    : state_(NULL)
{

}
AEEpoll::~AEEpoll()
{

}
// 接口实现
// 构造函数
int AEEpoll::aeApiCreate(aeEventLoop *eventLoop)
{
    state_ = (aeApiState *)malloc(sizeof(aeApiState));

    if (!state_)
        return -1;
    state_->events_ = (struct epoll_event *)malloc(sizeof(struct epoll_event) * eventLoop->setsize);
    if (!state_->events_)
    {
        free(state_);
        return -1;
    }
    state_->epfd_ = epoll_create(1024); /* 1024 is just a hint for the kernel */
    if (state_->epfd_ == -1)
    {
        free(state_->events_);
        free(state_);
        return -1;
    }
    eventLoop->apidata = state_;
    return 0;
}
// 析构函数
void AEEpoll::aeApiFree(aeEventLoop *eventLoop)
{
    state_ = (aeApiState *)eventLoop->apidata;

    close(state_->epfd_);
    free(state_->events_);
    free(state_);
}
int AEEpoll::aeApiResize(aeEventLoop *eventLoop, int setsize)
{
    state_ = (aeApiState *)eventLoop->apidata;

    state_->events_ = (struct epoll_event *)realloc(state_->events_, sizeof(struct epoll_event) * setsize);
    return 0;
}
// 添加事件
int AEEpoll::aeApiAddEvent(aeEventLoop *eventLoop, int fd, int mask)
{
    state_ = (aeApiState *)eventLoop->apidata;
    struct epoll_event ee = {0}; /* avoid valgrind warning */
    /* If the fd was already monitored for some event, we need a MOD
     * operation. Otherwise we need an ADD operation. */
    int op = eventLoop->events[fd].mask == AE_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ee.events = 0;
    mask |= eventLoop->events[fd].mask; /* Merge old events */
    if (mask & AE_READABLE)
        ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE)
        ee.events |= EPOLLOUT;
    ee.data.fd = fd;
    if (epoll_ctl(state_->epfd_, op, fd, &ee) == -1)
        return -1;
    return 0;
}
// 删除事件
void AEEpoll::aeApiDelEvent(aeEventLoop *eventLoop, int fd, int delmask)
{
    state_ = (aeApiState *)eventLoop->apidata;
    struct epoll_event ee = {0}; /* avoid valgrind warning */
    int mask = eventLoop->events[fd].mask & (~delmask);

    ee.events = 0;
    if (mask & AE_READABLE)
        ee.events |= EPOLLIN;
    if (mask & AE_WRITABLE)
        ee.events |= EPOLLOUT;
    ee.data.fd = fd;
    if (mask != AE_NONE)
    {
        epoll_ctl(state_->epfd_, EPOLL_CTL_MOD, fd, &ee);
    }
    else
    {
        /* Note, Kernel < 2.6.9 requires a non null event pointer even for
         * EPOLL_CTL_DEL. */
        epoll_ctl(state_->epfd_, EPOLL_CTL_DEL, fd, &ee);
    }
}
// 事件就绪检查
int AEEpoll::aeApiPoll(aeEventLoop *eventLoop, struct timeval *tvp)
{
    state_ = (aeApiState *)eventLoop->apidata;
    int retval, numevents = 0;

    retval = epoll_wait(state_->epfd_, state_->events_, eventLoop->setsize,
                        tvp ? (tvp->tv_sec * 1000 + tvp->tv_usec / 1000) : -1);
    if (retval > 0)
    {
        int j;

        numevents = retval;
        for (j = 0; j < numevents; j++)
        {
            int mask = 0;
            struct epoll_event *e = state_->events_ + j;

            if (e->events & EPOLLIN)
                mask |= AE_READABLE;
            if (e->events & EPOLLOUT)
                mask |= AE_WRITABLE;
            if (e->events & EPOLLERR)
                mask |= AE_WRITABLE;
            if (e->events & EPOLLHUP)
                mask |= AE_WRITABLE;
            eventLoop->fired[j].fd = e->data.fd;
            eventLoop->fired[j].mask = mask;
        }
    }
    return numevents;
}
char *AEEpoll::aeApiName(void)
{
    return "epoll";
}
