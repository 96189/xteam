
// 产品接口类型
// 产品 - 消息处理器接口类型

#ifndef _PRODUCT_HPP_
#define _PRODUCT_HPP_

#include "msg.hpp"

class MsgProcessor 
{
public:
    MsgProcessor()
    {

    }
    virtual ~MsgProcessor()
    {

    }

    // 接口定义
    virtual void ProcessMsg(Msg *msg) = 0;
};

#endif