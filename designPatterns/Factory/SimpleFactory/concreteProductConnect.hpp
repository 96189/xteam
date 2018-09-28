
// 产品实例化类型
// 产品 - 连接相关的消息处理器

#ifndef _CONCRETE_PRODUCT_CONNECT_HPP_
#define _CONCRETE_PRODUCT_CONNECT_HPP_

#include "product.hpp"

void ProcessLogin(Msg *msg)
{

}

void ProcessLogout(Msg *msg)
{

}

void ProcessReconnect(Msg *msg)
{

}

class ConnectProcessor : public MsgProcessor 
{
public:

public:
    // 构造 析构
    ConnectProcessor()
    {

    }
    ~ConnectProcessor()
    {

    }
    // 接口实现
    virtual void ProcessMsg(Msg *msg)
    {

    }
};

#endif