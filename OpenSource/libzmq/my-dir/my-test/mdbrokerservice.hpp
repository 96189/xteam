
//

#ifndef _MD_BROKER_SERVICE_HPP_
#define _MD_BROKER_SERVICE_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string.h>

#include "mdp.h"

class Broker;
class Worker;
class Service 
{
public:
    Broker *broker_;
    char *name_;            // 服务名称
    zlist_t *requests_;     // 客户端请求的队列
    zlist_t *waiting_;      // 当前服务拥有的worker队列
    size_t workers_;        // 可用worker数

public:
// 构造 析构
    Service(Broker *broker, char *name)
        : broker_(broker), name_(name)
    {
        requests_ = zlist_new();
        waiting_ = zlist_new();
    }
    ~Service()
    {

    }
// 
    static void Destroy(void *argument)
    {
        Service *service = (Service*)argument;
        while (zlist_size(service->requests_))
        {
            zmsg_t *msg = (zmsg_t*)zlist_pop(service->requests_);
            zmsg_destroy(&msg);
        }
        zlist_destroy(&service->requests_);
        zlist_destroy(&service->waiting_);
        free(service->name_);
        delete service;
    }
//
    void PushBackMsg(zmsg_t *msg);
    zmsg_t *PopFrontMsg();
    void ProcessMsg(zmsg_t *msg);
};


#endif