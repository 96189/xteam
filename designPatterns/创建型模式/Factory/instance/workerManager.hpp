
// 产品类 - 接口定义

#ifndef _WORKER_MANAGER_HPP_
#define _WORKER_MANAGER_HPP_

#include "processFactory.hpp"

class WorkerManager
{
public:
    ProcessFactory *processFactory_;    // 工厂方法模式 - 工厂的接口类型指针
public:
    WorkerManager()
        : processFactory_(NULL)
    {

    }
    virtual ~WorkerManager()
    {
        if (processFactory_)
        {
            delete processFactory_;
        }
    }

    // 接口定义
    virtual int ProcessClientPacket(int cmd) = 0;
};

#endif