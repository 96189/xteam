#ifndef _MD_BROKER_HPP_
#define _MD_BROKER_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string.h>

#include "mdp.h"

#define HEARTBEAT_LIVENESS  3       //  3-5 is reasonable
#define HEARTBEAT_INTERVAL  2500    //  msecs
#define HEARTBEAT_EXPIRY    (HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS)

#define BROKER_ADDR "tcp://*:5555"
#define OK "200"
#define NOT_FOUND "404"
#define SERVER_ERROR "500"

class Worker;
class Service;
class Broker 
{
public:
    void *context_;         // zmq context
    void *socket_;          // socket for clients and workers
    int verbose_;           // 
    char *endpoint_;        // broker binds to this endpoint
    zhash_t *services_;     // 已知服务的哈希表
    zhash_t *workers_;      // 已知worker的哈希表
    zlist_t *waiting_;      // list of waiting workers
    int64_t heartbeat_at_;  // 心跳开始时间

public:
    // services_
    void AddService(char *name, Service *service);
    Service* InquireService(char *name);

    // workers_
    void AddWorker(char *id_string, Worker *worker);
    void RemoveWorker(char *id_string);
    Worker* InquireWorker(char *id_string);

    // waiting_
    void AddWaitWorker(Worker *worker);
    void RemoveWaitWorker(Worker *worker);
    Worker* FirstWaitWorker();
    Worker* NextWaitWorker();
public:
// 构造 析构
    Broker(int verbose)
        : verbose_(verbose), endpoint_(NULL)
    {
        context_ = zmq_init(1);
        socket_ = zmq_socket(this->context_, ZMQ_ROUTER);
        services_ = zhash_new();
        workers_ = zhash_new();
        waiting_ = zlist_new();
        heartbeat_at_ = zclock_time() + HEARTBEAT_INTERVAL;
    }
    ~Broker()
    {
        Destroy();
    }
    void Destroy()
    {
        zmq_term(this->context_);
        zhash_destroy(&this->services_);
        zhash_destroy(&this->workers_);
        zlist_destroy(&this->waiting_);
    }
// 
    // 将代理绑定到某地址并开始监听
    int Bind(const char *endpoint);
    // 清除waiting_队列中过期的worker
    void Purge();
    // 查找或者创建worker
    Worker* RequireWorker(zframe_t *identity);
    // 查找或者创建service
    Service* RequireService(zframe_t *servicename);
    // 将指定消息转发到某服务
    void DispatchMsg(zframe_t *servicename, zmsg_t *msg);
    // 处理内部服务
    void ServiceInternal(zframe_t *servicename, zmsg_t *msg);
    // 处理服务端消息
    void ProcessWorkerMsg(zframe_t *sender, zmsg_t *msg);
    // 处理客户端消息 
    void ProcessClientMsg(zframe_t *sender, zmsg_t *msg);
};

#endif