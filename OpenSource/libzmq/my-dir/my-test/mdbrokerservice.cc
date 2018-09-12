#include "mdbrokerservice.hpp"
#include "mdbroker.hpp"
#include "mdbrokerworker.hpp"

void Service::PushBackWorker(Worker *worker)
{
    ++workers_;
    zlist_append(this->waiting_, worker);
}
Worker* Service::PopFrontWorker()
{
    --workers_;
    return (Worker*)zlist_pop(this->waiting_);
}
void Service::PushBackMsg(zmsg_t *msg)
{
    zlist_append(this->requests_, msg);
}
zmsg_t *Service::PopFrontMsg()
{
    return (zmsg_t*)zlist_pop(this->requests_);
}
void Service::ProcessMsg(zmsg_t *msg)
{
    // 消息入队
    if (msg) {
        this->PushBackMsg(msg);
    }

    // 删除broker中所有过期的worker
    this->broker_->Purge();

    while (zlist_size(this->waiting_) && 
           zlist_size(this->requests_)) {
        Worker *worker = PopFrontWorker();
        this->broker_->RemoveWaitWorker(worker);
        zmsg_t *msg = PopFrontMsg();
        worker->Send (MDPW_REQUEST, NULL, msg);
        zmsg_destroy (&msg);
    }
}