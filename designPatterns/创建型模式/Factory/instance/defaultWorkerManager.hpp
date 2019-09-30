
// 产品类 - 具体类

#ifndef _DEFAULT_WORKER_MANAGER_HPP_
#define _DEFAULT_WORKER_MANAGER_HPP_

#include "workerConf.hpp"
#include "workerManager.hpp"
#include "processor.hpp"
#include "defaultProcessFactory.hpp"

#include <iostream>

// 具体的产品类
class DefaultWorkerManager : public WorkerManager
{
public:
    DefaultWorkerManager(WorkerConfig *pConfig)
        : WorkerManager()
    {
        processFactory_ = new DefaultProcessFactory();
    }
    ~DefaultWorkerManager()
    {

    }

    // 接口实现
    virtual int ProcessClientPacket(int cmd)
    {
        std::cout << "DefaultWorkerManager INTERFACE ProcessClientPacket" << std::endl;
        int ret = -1;
        Processor *pProcessor = processFactory_->GetProcessor(cmd);
        if (pProcessor)
        {
            ret = pProcessor->DoRequest(cmd);
        }
        else 
        {
            // log
        }
        return ret;
    }
};

#endif