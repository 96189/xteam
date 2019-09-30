
// 简单工厂模式 - 创建型模式

#ifndef _SIMPLE_FACTORY_HPP_
#define _SIMPLE_FACTORY_HPP_

#include "product.hpp"
#include "concreteProductConnect.hpp"
#include "concreteProductBuy.hpp"
#include "concreteProductBrowse.hpp"

// 简单工厂模式 - 静态工厂方法模式
class MsgProcessorFactory 
{
public:
    // 简单工厂根据消息类型实例化不同的消息处理器
    static MsgProcessor* CreateProcessor(int type)
    {
        MsgProcessor* pProcessor = NULL;
        switch (type)
        {
            case LOGIN:
            case LOGOUT:
            case RECONNECT:
                pProcessor = new ConnectProcessor();
                break;
            case BUY:
            case CANCELBUY:
                pProcessor = new BuyProcessor();
                break;
            case SAVE:
            case SEARCH:
            case ADD:
            case DELETE:
            case COMMENT:
                pProcessor = new BrowseProcessor();
                break;
            default:
                break;
        }
        return pProcessor;
    }
};

#endif
