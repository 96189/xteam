
// 定时器管理类

#ifndef _TIMER_MANAGER_HPP_
#define _TIMER_MANAGER_HPP_

#include "timerobj.hpp"
#include "timerInterface.hpp"
#include <map>
#include <list>
#include <cstddef>

// 获取当前时间 单位:us
int64_t getTimestamp(void) 
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (int64_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

class TimerManager : public TimerInterface 
{
private:
    std::map<int, Timer*> timertb_;
    std::list<Timer*> timerlist_;

public:
    TimerManager()
    {

    }
    ~TimerManager()
    {

    }
    //
    void StartTimer(int timerId, int second)
    {
        std::map<int, Timer*>::iterator it = timertb_.find(timerId);
        if (it != timertb_.end())
        {
            timertb_.erase(timerId);
        }

        int64_t exptime = getTimestamp() + second * 1000 * 1000;
        Timer *timer = new Timer(timerId, exptime);

        timertb_[timerId] = timer;
        timerlist_.push_back(timer);
    }
    void StopTimer(int timerId)
    {
        std::map<int, Timer*>::iterator it = timertb_.find(timerId);
        if (it == timertb_.end()) return;
        
        timerlist_.remove(it->second); 
        timertb_.erase(timerId);
    }
    void CheckExpired()
    {
        timerlist_.sort();
        std::list<Timer*>::iterator it = timerlist_.begin();
        
        int64_t now = getTimestamp();
        while (it != timerlist_.end() && now >= (*it)->exptime_)
        {
            cb_((*it)->id_);
            // 
            timertb_.erase((*it)->id_);
            it = timerlist_.erase(it); 
        }
    }
};

#endif