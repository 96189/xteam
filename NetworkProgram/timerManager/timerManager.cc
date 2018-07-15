#include "timerManager.h"

TimerManager::TimerManager()
{
    timerManagerMap_.clear();
}
TimerManager::~TimerManager()
{
    for (ManagerMap::iterator it = timerManagerMap_.begin(); it != timerManagerMap_.end(); ++it)
    {
        TimerObj* pTimer = it->second;
        if (pTimer)
        {
            pTimer->DetachFromContainer();
            DelTimerFromMap(pTimer->id_);
        }
    }
    timerManagerMap_.clear();
}

TimerObj* TimerManager::findTimer(int timerId)
{
    TimerObj* pTimerObj = NULL;
    ManagerMap::const_iterator it = timerManagerMap_.find(timerId);
    if (it != timerManagerMap_.end())
    {
        pTimerObj = it->second;
    }
    return pTimerObj;
}
void TimerManager::AddTimerToMap(int timerId, TimerObj* pObj)
{
    timerManagerMap_[timerId] = pObj;
}
void TimerManager::DelTimerFromMap(int timerId)
{
    timerManagerMap_.erase(timerId);
}

void TimerManager::StartTimer(int timerId, time_t second/*, TimeOutCallback& cb*/)
{
    TimerObj* timer = findTimer(timerId);
    if (timer)
    {
        timer->DetachFromContainer();
        DelTimerFromMap(timerId);
    }
    TimerObj *newTimer = new TimerObj(timerId, second/*, cb*/);
    log_boot("TimerManager::StartTimer timerId:%d second:%d newTimer:0x%p", timerId, second, newTimer);
    newTimer->AttachToContainer();
    AddTimerToMap(timerId, newTimer);
}
void TimerManager::StopTimer(int timerId)
{
    TimerObj* timer = findTimer(timerId);
    if (timer)
    {
        timer->DetachFromContainer();
        DelTimerFromMap(timerId);        
    }
}
void TimerManager::CheckExpired(time_t exptime)
{
    log_boot("TimerManager::CheckExpired exptime:%d", exptime);
    ContainerManager::Instance()->CheckExpired(exptime, callback_);
}

void TimerManager::SetTimerOutCallback(const TimeOutCallback& cb)
{
    callback_ = cb;
}