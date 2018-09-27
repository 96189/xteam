
//

#include "aeEventLoop.hpp"

AEEventLoop::AEEventLoop(int setsize)
{
    eventLoop_ = new aeEventLoop(setsize);
    aeApi_ = new AEEpoll(eventLoop_);
    int ret = aeApi_->aeApiCreate();
    assert(ret != -1);
    for (int i = 0; i < setsize; i++)
        eventLoop_->events[i].mask = AE_NONE;
}
AEEventLoop::~AEEventLoop()
{
    if (aeApi_)
    {
        aeApi_->aeApiFree();
    }
    if (eventLoop_)
    {
        delete eventLoop_;
    }
}
void AEEventLoop::aeStop()
{
    eventLoop_->stop = 1;
}
int AEEventLoop::aeCreateFileEvent(int fd, int mask,
                      aeFileProc *proc, void *clientData)
{
    if (fd >= eventLoop_->setsize)
    {
        errno = ERANGE;
        return AE_ERR;
    }
    aeFileEvent *fe = &eventLoop_->events[fd];

    if (aeApi_->aeApiAddEvent(fd, mask) == -1)
        return AE_ERR;
    fe->mask |= mask;
    // 关联读事件处理器
    if (mask & AE_READABLE)
        fe->rfileProc = proc;
    // 关联写事件处理器
    if (mask & AE_WRITABLE)
        fe->wfileProc = proc;
    fe->clientData = clientData;
    if (fd > eventLoop_->maxfd)
        eventLoop_->maxfd = fd;
    return AE_OK;
}
void AEEventLoop::aeDeleteFileEvent(int fd, int mask)
{
    if (fd >= eventLoop_->setsize)
        return;
    aeFileEvent *fe = &eventLoop_->events[fd];
    if (fe->mask == AE_NONE)
        return;

    /* We want to always remove AE_BARRIER if set when AE_WRITABLE
     * is removed. */
    if (mask & AE_WRITABLE)
        mask |= AE_BARRIER;

    aeApi_->aeApiDelEvent(fd, mask);
    fe->mask = fe->mask & (~mask);
    if (fd == eventLoop_->maxfd && fe->mask == AE_NONE)
    {
        /* Update the max fd */
        int j;

        for (j = eventLoop_->maxfd - 1; j >= 0; j--)
            if (eventLoop_->events[j].mask != AE_NONE)
                break;
        eventLoop_->maxfd = j;
    }
}
int AEEventLoop::aeGetFileEvents(int fd)
{
    if (fd >= eventLoop_->setsize)
        return 0;
    aeFileEvent *fe = &eventLoop_->events[fd];

    return fe->mask;
}
void AEEventLoop::aeGetTime(long *seconds, long *milliseconds)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    *seconds = tv.tv_sec;
    *milliseconds = tv.tv_usec / 1000;
}
void AEEventLoop::aeAddMillisecondsToNow(long long milliseconds, long *sec, long *ms)
{
    long cur_sec, cur_ms, when_sec, when_ms;

    aeGetTime(&cur_sec, &cur_ms);
    when_sec = cur_sec + milliseconds / 1000;
    when_ms = cur_ms + milliseconds % 1000;
    if (when_ms >= 1000)
    {
        when_sec++;
        when_ms -= 1000;
    }
    *sec = when_sec;
    *ms = when_ms;
}
long long AEEventLoop::aeCreateTimeEvent(long long milliseconds,
                            aeTimeProc *proc, void *clientData,
                            aeEventFinalizerProc *finalizerProc)
{
    long long id = eventLoop_->timeEventNextId++;
    aeTimeEvent *te;

    te = (aeTimeEvent *)malloc(sizeof(*te));
    if (te == NULL)
        return AE_ERR;
    te->id = id;
    aeAddMillisecondsToNow(milliseconds, &te->when_sec, &te->when_ms);
    te->timeProc = proc;
    te->finalizerProc = finalizerProc;
    te->clientData = clientData;
    te->prev = NULL;
    // 头插法
    te->next = eventLoop_->timeEventHead;
    if (te->next)
        te->next->prev = te;
    eventLoop_->timeEventHead = te;
    return id;
}
int AEEventLoop::aeDeleteTimeEvent(long long id)
{
    aeTimeEvent *te = eventLoop_->timeEventHead;
    // 遍历链表
    while (te)
    {
        if (te->id == id)
        {
            te->id = AE_DELETED_EVENT_ID;
            return AE_OK;
        }
        te = te->next;
    }
    return AE_ERR; /* NO event with the specified ID found */
}
aeTimeEvent *AEEventLoop::aeSearchNearestTimer()
{
    aeTimeEvent *te = eventLoop_->timeEventHead;
    aeTimeEvent *nearest = NULL;

    while (te)
    {
        if (!nearest || te->when_sec < nearest->when_sec ||
            (te->when_sec == nearest->when_sec && te->when_ms < nearest->when_ms))
            nearest = te;
        te = te->next;
    }
    return nearest;
}
int AEEventLoop::aeProcessEvents(int flags)
{
    int processed = 0, numevents;

    /* Nothing to do? return ASAP */
    if (!(flags & AE_TIME_EVENTS) && !(flags & AE_FILE_EVENTS))
        return 0;

    if (eventLoop_->maxfd != -1 ||
        ((flags & AE_TIME_EVENTS) && !(flags & AE_DONT_WAIT)))
    {
        int j;
        aeTimeEvent *shortest = NULL;
        struct timeval tv, *tvp;

        if (flags & AE_TIME_EVENTS && !(flags & AE_DONT_WAIT))
            // 获取到达当前事件最近的时间事件
            shortest = aeSearchNearestTimer();
        if (shortest)
        {
            long now_sec, now_ms;

            // 获取当前时间
            aeGetTime(&now_sec, &now_ms);
            tvp = &tv;

            // 计算时间差
            long long ms =
                (shortest->when_sec - now_sec) * 1000 +
                shortest->when_ms - now_ms;

            if (ms > 0)
            {
                tvp->tv_sec = ms / 1000;
                tvp->tv_usec = (ms % 1000) * 1000;
            }
            else
            {
                tvp->tv_sec = 0;
                tvp->tv_usec = 0;
            }
        }
        else
        {
            if (flags & AE_DONT_WAIT)
            {
                // 设置文件事件不阻塞
                tv.tv_sec = tv.tv_usec = 0;
                tvp = &tv;
            }
            else
            {
                /* Otherwise we can block */
                // 设置文件事件可以阻塞到直到有事件到达为止
                tvp = NULL; /* wait forever */
            }
        }

        numevents = aeApi_->aeApiPoll(tvp);

        /* After sleep callback. */
        if (eventLoop_->aftersleep != NULL && flags & AE_CALL_AFTER_SLEEP)
            eventLoop_->aftersleep(this);

        // 处理已经发生的文件事件
        // 遍历事件集合
        for (j = 0; j < numevents; j++)
        {
            aeFileEvent *fe = &eventLoop_->events[eventLoop_->fired[j].fd];
            int mask = eventLoop_->fired[j].mask;
            int fd = eventLoop_->fired[j].fd;
            int fired = 0; /* Number of events fired for current fd. */

            int invert = fe->mask & AE_BARRIER;
            // 可读事件处理
            if (!invert && fe->mask & mask & AE_READABLE)
            {
                fe->rfileProc(this, fd, fe->clientData, mask);
                fired++;
            }

            /* Fire the writable event. */
            // 可写事件处理
            if (fe->mask & mask & AE_WRITABLE)
            {
                if (!fired || fe->wfileProc != fe->rfileProc)
                {
                    fe->wfileProc(this, fd, fe->clientData, mask);
                    fired++;
                }
            }

            if (invert && fe->mask & mask & AE_READABLE)
            {
                if (!fired || fe->wfileProc != fe->rfileProc)
                {
                    fe->rfileProc(this, fd, fe->clientData, mask);
                    fired++;
                }
            }

            processed++;
        }
    }
    /* Check time events */
    // 处理已经发生的超时事件
    if (flags & AE_TIME_EVENTS)
        processed += processTimeEvents();

    return processed; /* return the number of processed file/time events */
}
int AEEventLoop::processTimeEvents()
{
    int processed = 0;
    aeTimeEvent *te;
    long long maxId;
    time_t now = time(NULL);

    /* If the system clock is moved to the future, and then set back to the
     * right value, time events may be delayed in a random way. Often this
     * means that scheduled operations will not be performed soon enough.
     *
     * Here we try to detect system clock skews, and force all the time
     * events to be processed ASAP when this happens: the idea is that
     * processing events earlier is less dangerous than delaying them
     * indefinitely, and practice suggests it is. */
    if (now < eventLoop_->lastTime)
    {
        te = eventLoop_->timeEventHead;
        while (te)
        {
            te->when_sec = 0;
            te = te->next;
        }
    }
    // 更新最后一次处理时间事件的时间
    eventLoop_->lastTime = now;

    te = eventLoop_->timeEventHead;
    maxId = eventLoop_->timeEventNextId - 1;
    // 遍历整个事件链表
    while (te)
    {
        long now_sec, now_ms;
        long long id;

        /* Remove events scheduled for deletion. */
        if (te->id == AE_DELETED_EVENT_ID)
        {
            aeTimeEvent *next = te->next;
            if (te->prev)
                te->prev->next = te->next;
            else
                eventLoop_->timeEventHead = te->next;
            if (te->next)
                te->next->prev = te->prev;
            if (te->finalizerProc)
                te->finalizerProc(this, te->clientData);
            free(te);
            te = next;
            continue;
        }

        /* Make sure we don't process time events created by time events in
         * this iteration. Note that this check is currently useless: we always
         * add new timers on the head, however if we change the implementation
         * detail, this check may be useful again: we keep it here for future
         * defense. */
        if (te->id > maxId)
        {
            te = te->next;
            continue;
        }
        // 获取当前时间
        aeGetTime(&now_sec, &now_ms);
        // 判断当前节点的时间事件是否到期
        if (now_sec > te->when_sec ||
            (now_sec == te->when_sec && now_ms >= te->when_ms))
        {
            int retval;

            id = te->id;
            // 调用超时处理函数(时间事件处理器)
            retval = te->timeProc(this, id, te->clientData);
            processed++;
            // 周期性事件
            if (retval != AE_NOMORE)
            {
                aeAddMillisecondsToNow(retval, &te->when_sec, &te->when_ms);
                // 定时事件
            }
            else
            {
                te->id = AE_DELETED_EVENT_ID;
            }
        }
        te = te->next;
    }
    return processed;
}
int AEEventLoop::aeWait(int fd, int mask, long long milliseconds)
{
    struct pollfd pfd;
    int retmask = 0, retval;

    memset(&pfd, 0, sizeof(pfd));
    pfd.fd = fd;
    if (mask & AE_READABLE)
        pfd.events |= POLLIN;
    if (mask & AE_WRITABLE)
        pfd.events |= POLLOUT;

    if ((retval = poll(&pfd, 1, milliseconds)) == 1)
    {
        if (pfd.revents & POLLIN)
            retmask |= AE_READABLE;
        if (pfd.revents & POLLOUT)
            retmask |= AE_WRITABLE;
        if (pfd.revents & POLLERR)
            retmask |= AE_WRITABLE;
        if (pfd.revents & POLLHUP)
            retmask |= AE_WRITABLE;
        return retmask;
    }
    else
    {
        return retval;
    }
}
void AEEventLoop::aeMain()
{
    eventLoop_->stop = 0;
    while (!eventLoop_->stop)
    {
        if (eventLoop_->beforesleep != NULL)
            eventLoop_->beforesleep(this);
        // 执行事件分派
        aeProcessEvents(AE_ALL_EVENTS | AE_CALL_AFTER_SLEEP);
    }
}
char *AEEventLoop::aeGetApiName(void)
{
    return aeApi_->aeApiName();
}
void AEEventLoop::aeSetBeforeSleepProc(aeBeforeSleepProc *beforesleep)
{
    eventLoop_->beforesleep = beforesleep;
}
void AEEventLoop::aeSetAfterSleepProc(aeBeforeSleepProc *aftersleep)
{
    eventLoop_->aftersleep = aftersleep;
}
int AEEventLoop::aeGetSetSize()
{
    return eventLoop_->setsize;
}
int AEEventLoop::aeResizeSetSize(int setsize)
{
    int i;

    if (setsize == eventLoop_->setsize)
        return AE_OK;
    if (eventLoop_->maxfd >= setsize)
        return AE_ERR;
    if (aeApi_->aeApiResize(setsize) == -1)
        return AE_ERR;

    eventLoop_->events = (aeFileEvent *)realloc(eventLoop_->events, sizeof(aeFileEvent) * setsize);
    eventLoop_->fired = (aeFiredEvent *)realloc(eventLoop_->fired, sizeof(aeFiredEvent) * setsize);
    eventLoop_->setsize = setsize;

    /* Make sure that if we created new slots, they are initialized with
     * an AE_NONE mask. */
    for (i = eventLoop_->maxfd + 1; i < setsize; i++)
        eventLoop_->events[i].mask = AE_NONE;
    return AE_OK;
}