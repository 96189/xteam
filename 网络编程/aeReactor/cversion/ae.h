/* A simple event-driven programming library. Originally I wrote this code
 * for the Jim's event-loop (Jim is a Tcl interpreter) but later translated
 * it in form of a library for easy reuse.
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __AE_H__
#define __AE_H__

#include <time.h>

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

#if defined(__cplusplus)
extern "C" {
#endif

struct aeEventLoop;

/* Types and data structures */
// 回调定义
typedef void aeFileProc(struct aeEventLoop *eventLoop, int fd, void *clientData, int mask);
typedef int aeTimeProc(struct aeEventLoop *eventLoop, long long id, void *clientData);
typedef void aeEventFinalizerProc(struct aeEventLoop *eventLoop, void *clientData);
typedef void aeBeforeSleepProc(struct aeEventLoop *eventLoop);

// 文件事件
/* File event structure */
typedef struct aeFileEvent {
    // 事件类型
    int mask;                           /* one of AE_(READABLE|WRITABLE|BARRIER) */
    // 可读事件处理器(多态接口)
    aeFileProc *rfileProc;
    // 可写事件处理器(多态接口)
    aeFileProc *wfileProc;
    void *clientData;                   // 回调参数
} aeFileEvent;

// 时间事件
/* Time event structure */
typedef struct aeTimeEvent {
    // 全局唯一标识
    long long id;                       /* time event identifier. */
    long when_sec;                      /* seconds */
    long when_ms;                       /* milliseconds */
    // 时间事件处理器
    aeTimeProc *timeProc;
    // 时间事件析构函数
    aeEventFinalizerProc *finalizerProc;
    void *clientData;                   // 回调参数
    // 事件链式结构
    struct aeTimeEvent *prev;
    struct aeTimeEvent *next;
} aeTimeEvent;

// 发生的事件
/* A fired event */
typedef struct aeFiredEvent {
    int fd;
    int mask;                           // 事件类型
} aeFiredEvent;

// reactor
/* State of an event based program */
typedef struct aeEventLoop {
    int maxfd;                          /* highest file descriptor currently registered */
    int setsize;                        /* max number of file descriptors tracked */
    long long timeEventNextId;
    time_t lastTime;                    /* Used to detect system clock skew */
    aeFileEvent *events;                // 注册的事件数组 按描述符fd访问数组
    aeFiredEvent *fired;                // 发生的事件数组 按描述符fd访问数组
    aeTimeEvent *timeEventHead;         // 定时器链表 管理时间事件(可优化-链表查询效率较低)
    int stop;
    void *apidata;                      // 具体的aeApiState可能是epoll/select/poll相关数据
    aeBeforeSleepProc *beforesleep;     // epoll_wait前执行
    aeBeforeSleepProc *aftersleep;      // epoll_wait后执行
} aeEventLoop;

/* Prototypes */
// 创建/删除reactor
aeEventLoop *aeCreateEventLoop(int setsize);
void aeDeleteEventLoop(aeEventLoop *eventLoop);
void aeStop(aeEventLoop *eventLoop);

// 添加/删除/获取文件事件
int aeCreateFileEvent(aeEventLoop *eventLoop, int fd, int mask, aeFileProc *proc, void *clientData);
void aeDeleteFileEvent(aeEventLoop *eventLoop, int fd, int mask);
int aeGetFileEvents(aeEventLoop *eventLoop, int fd);

// 添加/删除时间事件
long long aeCreateTimeEvent(aeEventLoop *eventLoop, long long milliseconds, aeTimeProc *proc, void *clientData, aeEventFinalizerProc *finalizerProc);
int aeDeleteTimeEvent(aeEventLoop *eventLoop, long long id);

// 事件处理
int aeProcessEvents(aeEventLoop *eventLoop, int flags);

// 等待描述符fd的指定事件mask在milliseconds是否发生
int aeWait(int fd, int mask, long long milliseconds);
// reactor循环
void aeMain(aeEventLoop *eventLoop);
char *aeGetApiName(void);

// 设置事件处理前后的回调
void aeSetBeforeSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *beforesleep);
void aeSetAfterSleepProc(aeEventLoop *eventLoop, aeBeforeSleepProc *aftersleep);

// 获取/设置文件描述符数
int aeGetSetSize(aeEventLoop *eventLoop);
int aeResizeSetSize(aeEventLoop *eventLoop, int setsize);

#if defined(__cplusplus)
}
#endif

#endif
