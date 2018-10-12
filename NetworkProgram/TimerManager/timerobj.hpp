
// 定时器基类

#ifndef _TIMER_OBJ_HPP_
#define _TIMER_OBJ_HPP_

#include <sys/time.h>
#include <stdint.h>

class Timer 
{
public:
    int id_;
    int64_t exptime_;
    
public:
    Timer(int id, int64_t exptime)
        : id_(id), exptime_(exptime)
    {

    }
    virtual ~Timer()
    {

    }

    bool operator==(const Timer& timer)
    {
        return this->id_ == timer.id_;
    }
};

#endif
