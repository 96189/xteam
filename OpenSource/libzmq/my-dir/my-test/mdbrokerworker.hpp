
//

#ifndef _MD_BROKER_WORKER_HPP_
#define _MD_BROKER_WORKER_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string.h>

#include "mdp.h"

class Broker;
class Service;
class Worker 
{
public:
    Broker *broker_;
    char *id_string_;       // worker标识的可打印版本
    zframe_t *identity_;    // worker的标识
    Service *service_;      // 所属服务
    int64_t expiry_;        // 过期时间

public:
// 构造 析构
    Worker(Broker *broker, char *id_string, zframe_t *identity)
        : broker_(broker), id_string_(id_string), identity_(zframe_dup(identity))
    {

    }
    ~Worker()
    {

    }
// 
    static void Destroy(void *argument)
    {
        Worker *self = (Worker*)argument;
        zframe_destroy(&self->identity_);
        free(self->id_string_);
        delete self;
    }
    void Delete(int disconnect);
    void Send(char *command, char *option, zmsg_t *msg);
    void Waiting();
};



#endif