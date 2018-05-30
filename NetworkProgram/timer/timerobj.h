#include "clientdata.h"
#include <sys/time.h>
#include <functional>

typedef std::function<void(int)> TimeOutCallback;

// us
static inline time_t GET_TIMESTAMP(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (time_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

class TimerObj
{
  public:
    TimerObj(time_t delta)
    {
        // 相对时间转绝对时间 us -> ms
        exptime_ = (GET_TIMESTAMP() + delta * 1000000) / 1000;
    }
    ~TimerObj()
    {
    }
    void SetTimeOutCallBack(const TimeOutCallback &func, int fd)
    {
        cb_func_ = func;
        fd_ = fd;
    }
    // algorithm find
    bool operator==(const TimerObj &tobj)
    {
        return (this->fd_) == (tobj.fd_);
    }
    // container sort
    bool operator>(const TimerObj &tobj)
    {
        return this->exptime_ > tobj.exptime_;
    }
    bool operator<(const TimerObj &tobj)
    {
        return this->exptime_ < tobj.exptime_;
    }

  private:
    time_t exptime_;

    TimeOutCallback cb_func_;
    int fd_;

    friend class TimerManager;
};