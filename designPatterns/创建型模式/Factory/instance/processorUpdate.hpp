
// 产品类 - 具体的产品

#ifndef _PROCESSOR_UPDATE_HPP_
#define _PROCESSOR_UPDATE_HPP_

#include "processor.hpp"
#include <iostream>

// 更新请求处理器 
class UpdateProcessor : public Processor 
{
public:
    UpdateProcessor()
    {

    }
    ~UpdateProcessor()
    {

    }
public:
    // 接口实现
    virtual int DoRequest(int cmd)
    {
        std::cout << "UpdateProcessor DoRequest" << std::endl;
        return 0;
    }
};

#endif