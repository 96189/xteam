#ifndef _TIMER_CONTAINER_H_
#define _TIMER_CONTAINER_H_

#include <list>
#include "timerobj.h"
#include "interface.h"

// 一个TimerList保存的TimerObj对象的exptime_相同
class TimerList
{
private:
    std::list<TimerObj> timerlist_;     // TimerObj链表,二级链表
public:
    TimerList();
    ~TimerList();

    // 获取当前链表中保存的TimerObj的exptime_值(同一个链表中保存的exptime_相同)
    time_t GetTime();
    // 将TimerObj对象保存到当前链表末尾
    void Attach(const TimerObj& tobj);
    // 从当前链表中删除指定的TimerObj对象
    void Erase(const TimerObj& tobj);
    // 
    // std::list<TimerObj>::iterator FindTimerObjPtr(int timerId);
    bool Detach(const TimerObj& tobj);
    // 
    void CheckExpired(time_t exptime, const TimeOutCallback& cbfunc);
};

#endif