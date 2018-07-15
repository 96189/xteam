#include "timerobj.h"
#include "timerContainerManager.h"

TimerObj::TimerObj(int timerId, time_t exptime/*, TimeOutCallback &cb*/)
    : id_(timerId), exptime_(exptime)//, timeoutCallback_(cb)
{

}
TimerObj::~TimerObj()
{

}
// 
void TimerObj::AttachToContainer()
{
    ContainerManager::Instance()->Attach(this);
}
// 
void TimerObj::DetachFromContainer()
{
    ContainerManager::Instance()->Detach(this);
}

bool TimerObj::operator==(const TimerObj& rhs)
{
    return this->id_ == rhs.id_;
}