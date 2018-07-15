#ifndef _TIMER_CONTAINER_MANAGER_H_
#define _TIMER_CONTAINER_MANAGER_H_

#include <list>
#include "timerContainer.h"
#include "singleton.h"
#include "interface.h"

typedef TimerList Container;

typedef std::list<Container*> ContainerManagerList;

class ContainerManager
{
private:
    ContainerManagerList containerManager_;
    ContainerManager();
    ~ContainerManager();
public:
	static ContainerManager* Instance(void);
	static void Destroy (void);
public:
    void Attach(TimerObj* pTimer);
    void Detach(TimerObj* pTimer);
    void CheckExpired(time_t exptime, const TimeOutCallback& cbfunc);

    Container* FindContainerPtr(TimerObj* pTimer);
    friend class  CreateUsingNew<ContainerManager>;
};

#endif