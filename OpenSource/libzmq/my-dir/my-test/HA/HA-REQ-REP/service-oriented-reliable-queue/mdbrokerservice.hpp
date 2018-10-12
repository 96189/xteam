
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
    char *name_;                // 服务名称
    zlist_t *requestqueue_;     // 客户端请求的队列 (zmsg_t)
    zlist_t *workerqueue_;      // 当前服务拥有的worker队列 (Worker)

public:
// 构造 析构
    Service(Broker *broker, char *name)
        : broker_(broker), name_(name)
    {
        requestqueue_ = zlist_new();
        workerqueue_ = zlist_new();
    }
    ~Service()
    {

    }
// 
    static void Destroy(void *argument)
    {
        Service *service = (Service*)argument;
        while (zlist_size(service->requestqueue_))
        {
            zmsg_t *msg = (zmsg_t*)zlist_pop(service->requestqueue_);
            zmsg_destroy(&msg);
        }
        zlist_destroy(&service->requestqueue_);
        zlist_destroy(&service->workerqueue_);
        free(service->name_);
        delete service;
    }
//
    void WorkerQueuePush(Worker *worker);
    Worker* WorkerQueuePop();
    size_t WorkerQueueSize();

    void RequestQueuePush(zmsg_t *msg);
    zmsg_t *RequestQueuePop();
    size_t RequestQueueSize();

    void ProcessMsg(zmsg_t *msg);
};


#endif