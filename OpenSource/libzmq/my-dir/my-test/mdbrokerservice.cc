#include "mdbrokerservice.hpp"
#include "mdbroker.hpp"
#include "mdbrokerworker.hpp"

void Service::WorkerQueuePush(Worker *worker)
{
    zlist_append(this->workerqueue_, worker);
}
Worker* Service::WorkerQueuePop()
{
    return (Worker*)zlist_pop(this->workerqueue_);
}
size_t Service::WorkerQueueSize()
{
    return zlist_size(this->workerqueue_);
}

void Service::RequestQueuePush(zmsg_t *msg)
{
    zlist_append(this->requestqueue_, msg);
}
size_t Service::RequestQueueSize()
{
    return zlist_size(this->requestqueue_);
}
zmsg_t *Service::RequestQueuePop()
{
    return (zmsg_t*)zlist_pop(this->requestqueue_);
}

void Service::ProcessMsg(zmsg_t *msg)
{
    // 消息入队
    if (msg) {
        this->RequestQueuePush(msg);
    }

    // 删除broker中所有过期的worker
    this->broker_->Purge();

    while (WorkerQueueSize() > 0 && 
           RequestQueueSize() > 0) {
        Worker *worker = WorkerQueuePop();
        this->broker_->WorkerQueuePop(worker);
        zmsg_t *msg = RequestQueuePop();
        worker->Send(MDPW_REQUEST, NULL, msg);
        zmsg_destroy(&msg);
    }
}