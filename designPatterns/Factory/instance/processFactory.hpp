
// 工厂方法模式 - 工厂接口

#ifndef _PROCESS_FACTORY_HPP_
#define _PROCESS_FACTORY_HPP_

#include <map>
#include "processor.hpp"

// 命令字处理器工厂接口类
class ProcessFactory 
{
public:
    std::map<int, Processor*> processorMap_;    // cmd -> processor*
public:
    ProcessFactory()
    {

    }
    virtual ~ProcessFactory()
    {

    }
    // 查表获取
    Processor* GetProcessor(int cmd)
    {
        Processor* pProcessor = NULL;
        std::map<int, Processor*>::iterator it = processorMap_.find(cmd);
        if (it == processorMap_.end())
        {
            pProcessor = CreateProcessor(cmd);
            if (pProcessor)
            {
                processorMap_[cmd] = pProcessor;
            }
        }
        else 
        {
            pProcessor = it->second;
        }
        return pProcessor;
    }
    // 接口定义
    virtual Processor* CreateProcessor(int cmd) = 0;
};

#endif