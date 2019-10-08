
// 定时器操作接口

#ifndef _TIMER_INTERFACE_HPP_
#define _TIMER_INTERFACE_HPP_

typedef void (*TimeOutCallback)(int timerId);

class TimerInterface
{
public:
    TimeOutCallback cb_;
public:
    TimerInterface()
    {

    }
    virtual ~TimerInterface()
    {
        
    }
    void RegisterTimerOutCallback(TimeOutCallback cb)
    {
        cb_ = cb;
    }
public:
    virtual void StartTimer(int timerId, int second) = 0;
    virtual void StopTimer(int timerId) = 0;
    virtual void CheckExpired() = 0;
};

#endif