#ifndef _TIMER_MANAGER_H_
#define _TIMER_MANAGER_H_
#include <map>
#include "timerobj.h"
#include "timerContainerManager.h"
#include "interface.h"

// typedef void (*TimeOutCallback)(int timerId);

// timerId -> TimerObj*
typedef std::map<int, TimerObj*> ManagerMap;

class TimerManager
{
private:
    friend class TimerObj;
    ManagerMap timerManagerMap_;
    TimeOutCallback callback_;
private:
    TimerObj* findTimer(int timerId);
    void AddTimerToMap(int timerId, TimerObj* pObj);
    void DelTimerFromMap(int timerId);
public:
    TimerManager();
    ~TimerManager();
public:
    void StartTimer(int timerId, time_t second/*, const TimeOutCallback& cb*/);
    void StopTimer(int timerId);
    void CheckExpired(time_t exptime);
    void SetTimerOutCallback(const TimeOutCallback& cb);
};

#endif