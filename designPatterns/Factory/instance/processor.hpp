
// 产品类 - 接口类

#ifndef _PROCESSOR_HPP_
#define _PROCESSOR_HPP_

// 命令字处理器接口类
class Processor 
{
public:
    // 构造 析构
    Processor()
    {

    }
    virtual ~Processor()
    {

    }

    // 接口定义
    virtual int DoRequest(int cmd) = 0;
};

#endif
