#ifndef _TIMER_OBJ_H_
#define _TIMER_OBJ_H_
#include <sys/time.h>

class TimerObj
{
public:
    int id_;            // timerId
    time_t exptime_;    // ms
    // TimeOutCallback timeoutCallback_;
public:
    TimerObj(int timerId, time_t exptime/*, TimeOutCallback& cb*/);
    ~TimerObj();
    // 添加到二级链表中
    void AttachToContainer();
    // 从二级链表中删除
    void DetachFromContainer();
    // 
    bool operator==(const TimerObj& rhs);
};

#endif
