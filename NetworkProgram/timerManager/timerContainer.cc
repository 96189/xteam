#include "timerContainer.h"
# include <algorithm>

TimerList::TimerList()
{

}
TimerList::~TimerList()
{

}
void TimerList::Attach(const TimerObj& tobj)
{
    timerlist_.push_back(tobj);
    log_boot("TimerList::Attach tobj:0x%p to timerlist_:0x%p", &tobj, &timerlist_);
}
void TimerList::Erase(const TimerObj& tobj)
{
    std::list<TimerObj>::iterator it = find(timerlist_.begin(), timerlist_.end(), tobj);
    if (it != timerlist_.end())
    {
        timerlist_.erase(it);
    }
    // TimerObj* pTimer = FindTimerObjPtr(tobj.id_);
    // if (pTimer)
    // {
    //     timerlist_.erase(tobj);
    // }
}
bool TimerList::Detach(const TimerObj& tobj)
{
    bool detached = false;
    std::list<TimerObj>::iterator it = find(timerlist_.begin(), timerlist_.end(), tobj);
    if (it != timerlist_.end())
    {
        detached = true;
        // 容器erase析构TimerObj
        timerlist_.erase(it);
        log_boot("TimerList::Detach erase tobj:0x%p from timerlist_:0x%p", &tobj, &timerlist_);
    }
    return detached;
}
time_t TimerList::GetTime()
{
    std::list<TimerObj>::const_iterator it = timerlist_.begin();
    if (it != timerlist_.end())
    {
        return it->exptime_;
    }
    return -1;
}
// std::list<TimerObj>::iterator TimerList::FindTimerObjPtr(TimerObj* pTimerObj)
// {
//     // for (std::list<TimerObj>::iterator it = timerlist_.begin(); it != timerlist_.end(); ++it)
//     // {
//     //     if (*it->id_ == timerId)
//     //     {
//     //         return &(*it);
//     //     }
//     // }
//     std::list<TimerObj>::iterator it = find(timerlist_.begin(), timerlist_.end(), *pTimerObj);
//     return it;
// }
void TimerList::CheckExpired(time_t exptime, const TimeOutCallback& cbfunc)
{
    for (std::list<TimerObj>::iterator it = timerlist_.begin(); it != timerlist_.end(); ++it)
    {
        if (it->exptime_ <= exptime)
        {
            // cb_func
            // cbfunc(it->id_);
            log_boot("TimerList::CheckExpired exptime:%d TimerObj:0x%p", exptime, *it);
            cbfunc(it->id_);
            // 两种方案: 目前用1 
            // 1、 StartTimer() 必须 手动StopTimer()
            // 2、 StartTimer()不需要手动StopTimer()在这里超时直接释放内存
        }
    }
}