
//

#ifndef _AE_EVENT_LOOP_HPP_
#define _AE_EVENT_LOOP_HPP_

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <assert.h>
#include "aeApi.hpp"
#include "aeEpoll.hpp"

#define AE_OK 0
#define AE_ERR -1

#define AE_NONE 0       /* No events registered. */
// 客户端对套接字执行write close connect时,服务端变为可读
#define AE_READABLE 1   /* Fire when descriptor is readable. */
// 客户端对套接字执行read,服务端变为可写
#define AE_WRITABLE 2   /* Fire when descriptor is writable. */
#define AE_BARRIER 4    /* With WRITABLE, never fire the event if the
                           READABLE event already fired in the same event
                           loop iteration. Useful when you want to persist
                           things to disk before sending replies, and want
                           to do that in a group fashion. */

#define AE_FILE_EVENTS 1
#define AE_TIME_EVENTS 2
#define AE_ALL_EVENTS (AE_FILE_EVENTS|AE_TIME_EVENTS)
#define AE_DONT_WAIT 4
#define AE_CALL_AFTER_SLEEP 8

#define AE_NOMORE -1
#define AE_DELETED_EVENT_ID -1

/* Macros */
#define AE_NOTUSED(V) ((void) V)

class AEEventLoop;

/* Types and data structures */
typedef void aeFileProc(AEEventLoop *eventLoop, int fd, void *clientData, int mask);
typedef int aeTimeProc(AEEventLoop *eventLoop, long long id, void *clientData);
typedef void aeEventFinalizerProc(AEEventLoop *eventLoop, void *clientData);
typedef void aeBeforeSleepProc(AEEventLoop *eventLoop);

/* File event structure */
typedef struct aeFileEvent {
    // 事件类型
    int mask; /* one of AE_(READABLE|WRITABLE|BARRIER) */
    // 可读事件处理器(多态接口)
    aeFileProc *rfileProc;
    // 可写事件处理器(多态接口)
    aeFileProc *wfileProc;
    void *clientData;
} aeFileEvent;

/* Time event structure */
typedef struct aeTimeEvent {
    // 全局唯一标识
    long long id; /* time event identifier. */
    long when_sec; /* seconds */
    long when_ms; /* milliseconds */
    // 事件事件处理器
    aeTimeProc *timeProc;
    // 事件事件析构函数
    aeEventFinalizerProc *finalizerProc;
    void *clientData;
    // 事件链式结构
    struct aeTimeEvent *prev;
    struct aeTimeEvent *next;
} aeTimeEvent;

/* A fired event */
typedef struct aeFiredEvent {
    int fd;
    int mask;
} aeFiredEvent;

class aeEventLoop 
{
public:
    int maxfd;   /* highest file descriptor currently registered */
    int setsize; /* max number of file descriptors tracked */
    long long timeEventNextId;
    time_t lastTime;     /* Used to detect system clock skew */
    aeFileEvent *events; /* Registered events */
    aeFiredEvent *fired; /* Fired events */
    aeTimeEvent *timeEventHead;
    int stop;
    void *apidata; /* This is used for polling API specific data */
    aeBeforeSleepProc *beforesleep;
    aeBeforeSleepProc *aftersleep;

public:
    aeEventLoop(int sz)
    {
        setsize = sz;
        events = (aeFileEvent *)malloc(sizeof(aeFileEvent) * setsize);
        fired = ( aeFiredEvent *)malloc(sizeof(aeFiredEvent) * setsize);
        assert(events != NULL && fired != NULL);
        lastTime = time(NULL);
        timeEventHead = NULL;
        timeEventNextId = 0;
        stop = 0;
        maxfd = -1;
        beforesleep = NULL;
        aftersleep = NULL;
    }
    ~aeEventLoop()
    {
        free(events);
        free(fired);
    }
};


class AEEventLoop
{
private:
    aeEventLoop *eventLoop_;
    AEApi *aeApi_;
public:
    AEEventLoop(int setsize);
    ~AEEventLoop();
    void aeStop();
    int aeCreateFileEvent(int fd, int mask,
                          aeFileProc *proc, void *clientData);
    void aeDeleteFileEvent(int fd, int mask);
    int aeGetFileEvents(int fd);
    void aeGetTime(long *seconds, long *milliseconds);
    void aeAddMillisecondsToNow(long long milliseconds, long *sec, long *ms);
    long long aeCreateTimeEvent(long long milliseconds,
                                aeTimeProc *proc, void *clientData,
                                aeEventFinalizerProc *finalizerProc);
    int aeDeleteTimeEvent(long long id);
    aeTimeEvent *aeSearchNearestTimer(aeEventLoop *eventLoop);
    int aeProcessEvents(int flags);
    int processTimeEvents(aeEventLoop *eventLoop);
    int aeWait(int fd, int mask, long long milliseconds);
    void aeMain();
    char *aeGetApiName(void);
    void aeSetBeforeSleepProc(aeBeforeSleepProc *beforesleep);
    void aeSetAfterSleepProc(aeBeforeSleepProc *aftersleep);
    int aeGetSetSize();
    int aeResizeSetSize(int setsize);
};

#endif