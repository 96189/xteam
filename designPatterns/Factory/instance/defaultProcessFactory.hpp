
// 工厂方法模式 - 具体的工厂类

#ifndef _DEFAULT_PROCESS_FACTORY_HPP_
#define _DEFAULT_PROCESS_FACTORY_HPP_

#include "processor.hpp"
#include "processFactory.hpp"
#include "processorUpdate.hpp"
#include <iostream>

#define UPDATE_USE_DEFAULT 0

// 默认的命令字处理工厂
class DefaultProcessFactory : public ProcessFactory
{
public:
    virtual Processor* CreateProcessor(int cmd)
    {
        Processor *pProcessor = NULL;
        switch (cmd)
        {
            case UPDATE_USE_DEFAULT:
                std::cout << "DefaultProcessFactory CreateProcessor UPDATE_USE_DEFAULT " << std::endl;
                pProcessor = new UpdateProcessor();
                break;
            default:
                break;
        }
        return pProcessor;
    }
};

#endif