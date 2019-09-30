
//

#ifndef _OLD_PROCESS_MSG_HPP_
#define _OLD_PROCESS_MSG_HPP_

#include "msg.hpp"

void processLogin(Msg *msg)
{

}

void processLogout(Msg *msg)
{

}

void processReconnect(Msg *msg)
{

}

void processComment(Msg *msg)
{

}

void processBuy(Msg *msg)
{

}

void processCancelBuy(Msg *msg) 
{

}

void processSave(Msg *msg)
{

}

void processSearch(Msg *msg)
{

}

void processAdd(Msg *msg)
{

}

void processDelete(Msg *msg)
{

}

void OldProcessMsg(Msg *msg)
{
    switch(msg->type_)
    {
        case LOGIN:
            processLogin(msg);
            break;
        case LOGOUT:
            processLogout(msg);
            break;
        case RECONNECT:
            processReconnect(msg);
            break;
        case COMMENT:
            processComment(msg);
            break;
        case BUY:
            processBuy(msg);
            break;
        case CANCELBUY:
            processCancelBuy(msg);
            break;
        case SAVE:
            processSave(msg);
            break;
        case SEARCH:
            processSearch(msg);
            break;
        case ADD:
            processAdd(msg);
            break;
        case DELETE:
            processDelete(msg);
            break;
        
        // ...

        default:
            break;
            // log
    }
}

#endif