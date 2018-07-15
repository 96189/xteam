#include "timerContainerManager.h"

ContainerManager::ContainerManager()
{

}
ContainerManager::~ContainerManager()
{

}

ContainerManager* ContainerManager::Instance(void)
{
    return CSingleton<ContainerManager>::Instance();
}
void ContainerManager::Destroy(void)
{
    return CSingleton<ContainerManager>::Destroy();
}


Container* ContainerManager::FindContainerPtr(TimerObj* pTimer)
{
    Container* pTimerList = NULL;
    for (ContainerManagerList::iterator it = containerManager_.begin(); it != containerManager_.end(); ++it)
    {
        if ((*it)->GetTime() == pTimer->exptime_)
        {
            pTimerList = *it;
            break;
        }
    }    
    return pTimerList;
}
void ContainerManager::Attach(TimerObj *pTimer)
{
    // 
    Container* pTimerList = FindContainerPtr(pTimer);
    if (pTimerList)
    {
        pTimerList->Attach(*pTimer);
    }
    else
    {
        TimerList *newTimerList = new TimerList();
        log_boot("ContainerManager::Attach timer:0x%p newTimerList:0x%p", pTimer, newTimerList);
        newTimerList->Attach(*pTimer);
        containerManager_.push_back(newTimerList);
        log_boot("ContainerManager::Attach newTimerList:0x%p to containerManager_:0x%p", newTimerList, &containerManager_);
    }
}
void ContainerManager::Detach(TimerObj *pTimer)
{
    Container* pTimerList = FindContainerPtr(pTimer);
    if (pTimerList)
    {
        // TimerObj* pTimerObj = pTimerList->FindTimerObjPtr(pTimer);
        // if (pTimerObj)
        // {
        //     pTimerList->Erase(*pTimerObj);
        //     delete pTimerObj;
        // }

        if (pTimerList->Detach(*pTimer))
        {
            log_boot("ContainerManager::Detach pTimer:0x%p detach from pTimerList:0x%p", pTimer, pTimerList);
            // todo 从一级链表中删除二级链表
            // delete pTimer;
        }
    }
}

void ContainerManager::CheckExpired(time_t exptime, const TimeOutCallback& cbfunc)
{
    for (ContainerManagerList::iterator it = containerManager_.begin(); it != containerManager_.end(); ++it)
    {
        if ((*it)->GetTime() <= exptime)
        {
            log_boot("ContainerManager::CheckExpired Container:0x%p exptime:%d", *it, exptime);
            (*it)->CheckExpired(exptime, cbfunc);
        }
        // else
        // {
        //     ++it;
        // }
    }
}