
// 产品实例化类型
// 产品 - 浏览相关的消息处理器

#ifndef _CONCRETE_PRODUCT_BROWSE_HPP_
#define _CONCRETE_PRODUCT_BROWSE_HPP_

#include "product.hpp"

void ProcessComment(Msg *msg)
{

}

void ProcessSave(Msg *msg)
{

}

void ProcessSearch(Msg *msg)
{

}

void ProcessAdd(Msg *msg)
{

}

void ProcessDelete(Msg *msg)
{

}

class BrowseProcessor : public MsgProcessor 
{

public:
    // 构造 析构
    BrowseProcessor()
    {

    }
    ~BrowseProcessor()
    {

    }
    // 接口实现
    virtual void ProcessMsg(Msg *msg)
    {
        
    }    
};

#endif