#include "aeEpoll.hpp"
#include "aeEventLoop.hpp"

// 构造 析构不做实际工作
AEEpoll::AEEpoll(aeEventLoop *eventLoop)
    : state_(NULL)
{
    eventLoop_ = eventLoop;
}
AEEpoll::~AEEpoll()
{

}
// 接口实现
// 构造函数
int AEEpoll::aeApiCreate()
{
    state_ = new aeApiState(eventLoop_->setsize); 
    return 0;
}
// 析构函数
void AEEpoll::aeApiFree()
{
    delete state_;
}
int AEEpoll::aeApiResize(int setsize)
{
    state_->events_ = (struct epoll_event *)realloc(state_->events_, sizeof(struct epoll_event) * setsize);
    return 0;
}
// 添加事件
int AEEpoll::aeApiAddEvent(int fd, int mask)
{
    struct epoll_event ee = {0}; /* avoid valgrind warning */
    /* If the fd was already monitored for some event, we need a MOD
     * operation. Otherwise we need an ADD operation. */
    int op = eventLoop_->events[fd].mask == AE_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

    ee.events = 0;
    mask |= eventLoop_->events[fd].mask; /* Merge old events */
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
void AEEpoll::aeApiDelEvent(int fd, int delmask)
{
    struct epoll_event ee = {0}; /* avoid valgrind warning */
    int mask = eventLoop_->events[fd].mask & (~delmask);

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
int AEEpoll::aeApiPoll(struct timeval *tvp)
{
    int retval, numevents = 0;

    retval = epoll_wait(state_->epfd_, state_->events_, eventLoop_->setsize,
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
            eventLoop_->fired[j].fd = e->data.fd;
            eventLoop_->fired[j].mask = mask;
        }
    }
    return numevents;
}
char *AEEpoll::aeApiName(void)
{
    return "epoll";
}
