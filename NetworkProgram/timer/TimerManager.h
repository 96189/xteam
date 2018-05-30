#ifndef _TIMER_MANAGER_H_
#define _TIMER_MANAGER_H_

#include <list>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <memory>
#include <assert.h>
#include "timerobj.h"

class TimerManager
{
  private:
    std::list<TimerObj> timerList_;

    void Register(const TimerObj *tobj)
    {
        timerList_.push_back(*tobj);
        timerList_.sort();
    }
    void Remove(std::list<TimerObj>::iterator it)
    {
        timerList_.erase(it);
    }
    // 超时处理
    void ProcessTimerout(TimerObj &tobj)
    {
        tobj.cb_func_(tobj.fd_);
    }

  public:
    TimerManager()
    {
        timerList_.clear();
    }
    ~TimerManager()
    {
        for (std::list<TimerObj>::iterator it = timerList_.begin(); it != timerList_.end(); ++it)
        {
            timerList_.erase(it);
        }
        timerList_.clear();
    }
    // 提供给外部接口
    int Size()
    {
        return timerList_.size();
    }
    bool Empty()
    {
        return timerList_.empty();
    }
    time_t GetMinExpireTime()
    {
        std::list<TimerObj>::iterator it = timerList_.begin();
        return it->exptime_;
    }
    void CheckExpired(time_t expire_time)
    {
        for (std::list<TimerObj>::iterator it = timerList_.begin(); it != timerList_.end();)
        {
            if (it->exptime_ <= expire_time)
            {
                printf("CheckExpired erase exptime_:%ld sockfd:%d\n\n", it->exptime_, it->fd_);
                ProcessTimerout(*it);
                it = timerList_.erase(it);
            }
            else
            {
                break;
            }
        }
    }
    void StartTimer(const TimerObj *tobj)
    {
        assert(NULL != tobj);
        std::list<TimerObj>::iterator it = find(timerList_.begin(), timerList_.end(), *tobj);
        if (it != timerList_.end())
        {
            printf("StartTimer erase exptime_:%ld sockfd:%d\n", it->exptime_, it->fd_);
            it = timerList_.erase(it);
        }
        Register(tobj);
        printf("StartTimer exptime_:%ld sockfd:%d\n", tobj->exptime_, tobj->fd_);
    }
    void StopTimer(const TimerObj *tobj)
    {
        assert(NULL != tobj);
        printf("StopTimer exptime_:%ld sockfd:%d\n", tobj->exptime_, tobj->fd_);
        std::list<TimerObj>::iterator it = find(timerList_.begin(), timerList_.end(), *tobj);
        if (it != timerList_.end())
        {
            printf("StopTimer erase exptime_:%ld sockfd:%d\n", it->exptime_, it->fd_);
            it = timerList_.erase(it);
        }
    }
};

#endif