
// 产品实例化类型
// 产品 - 支付相关的消息处理器

#ifndef _CONCRETE_PRODUCT_BUY_
#define _CONCRETE_PRODUCT_BUY_

#include "product.hpp"

void ProcessBuy(Msg *msg)
{

}

void ProcessCancelBuy(Msg *msg) 
{

}

class BuyProcessor : public MsgProcessor 
{

public:
    // 构造 析构
    BuyProcessor()
    {

    }
    ~BuyProcessor()
    {

    }
    // 接口实现
    virtual void ProcessMsg(Msg *msg)
    {
        
    }    
};

#endif