
//

#ifndef _PROCESS_MSG_HPP_
#define _PROCESS_MSG_HPP_

#include "msg.hpp"
#include "product.hpp"
#include "simpleFactory.hpp"

// 消息处理入口
void ProcessMsg(Msg *msg)
{
    // 简单工厂根据类型创建具体的消息处理器
    MsgProcessor* pProcessor = MsgProcessorFactory::CreateProcessor(msg->type_);
    if (pProcessor)
    {
        // 由对应的消息处理器来处理消息
        pProcessor->ProcessMsg(msg);
    }
}

#endif