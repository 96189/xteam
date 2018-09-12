
//
#include "mdbroker.hpp"
#include "mdbrokerservice.hpp"
#include "mdbrokerworker.hpp"

void Broker::AddService (char *name, Service *service)
{
    zhash_insert(this->services_, name, service);
    zhash_freefn(this->services_, name, service->Destroy);
}
Service* Broker::InquireService(char *name)
{
    return (Service*)zhash_lookup(this->services_, name);
}
void Broker::AddWorker (char *id_string, Worker *worker)
{
    zhash_insert(this->workers_, id_string, worker);
    zhash_freefn(this->workers_, id_string, worker->Destroy);
}
void Broker::RemoveWorker(char *id_string)
{
    zhash_delete(this->workers_, id_string);
}
Worker* Broker::InquireWorker(char *id_string)
{
    return (Worker*)zhash_lookup(this->workers_, id_string);
}

void Broker::AddWaitWorker(Worker *worker)
{
    zlist_append(this->waiting_, worker);
}
void Broker::RemoveWaitWorker(Worker *worker)
{
    zlist_remove(this->waiting_, worker);
}
Worker* Broker::FirstWaitWorker()
{
    return (Worker*)zlist_first(this->waiting_);
}
Worker* Broker::NextWaitWorker()
{
    return (Worker*)zlist_next(this->waiting_);
}
int Broker::Bind (const char *endpoint)
{
    this->endpoint_ = strdup(endpoint);
    zclock_log("I: MDP broker/0.2.0 is active at %s", endpoint);
    return zmq_bind (this->socket_, this->endpoint_);
}
// 查找或者创建worker
Worker *Broker::RequireWorker (zframe_t *identity)
{
    assert (identity);
    char *id_string = zframe_strhex(identity);
    // 查找
    Worker *worker = this->InquireWorker(id_string);
    if (worker == NULL) {
        // 创建
        worker = new Worker(this, id_string, identity);
        AddWorker(id_string, worker);
        if (this->verbose_) {
            zclock_log("I: registering new worker: %s", id_string);
        }
    } else {
        free(id_string);
    }
    return worker;
}
// 查找或者创建service
Service *Broker::RequireService (zframe_t *servicename)
{
    assert(servicename);
    char *name = zframe_strdup(servicename);
    // 查找
    Service *service = this->InquireService(name);
    if (service == NULL) {
        // 创建
        service = new Service(this, name);
        AddService(name, service);
        if (this->verbose_)
            zclock_log("I: added service: %s", name);
    } else {
        free(name);
    }
    return service;
}
void Broker::DispatchMsg(zframe_t *servicename, zmsg_t *msg)
{
    // 服务查询
    Service *service = this->RequireService(servicename);
    assert(service != NULL);
    // 消息处理
    service->ProcessMsg(msg);
}
// 处理内部服务
void Broker::ServiceInternal(zframe_t *servicename, zmsg_t *msg)
{
    const char *return_code;
    if (zframe_streq(servicename, "mmi.service")) {
        char *name = zframe_strdup(zmsg_last(msg));
        // 服务查询
        Service *service = this->InquireService(name);
        // 查询结果
        return_code = service && service->workers_ ? OK : NOT_FOUND;
        free(name);
    } else {
        return_code = SERVER_ERROR;
    }

    zframe_reset(zmsg_last(msg), return_code, strlen(return_code));

    zframe_t *client = zmsg_unwrap(msg);
    zmsg_prepend(msg, &servicename);
    zmsg_pushstr(msg, MDPC_CLIENT);
    zmsg_wrap(msg, client);
    zmsg_send(&msg, this->socket_);
}
// 处理服务端消息
void Broker::ProcessWorkerMsg(zframe_t *sender, zmsg_t *msg)
{
    assert (zmsg_size(msg) >= 1);

    zframe_t *command = zmsg_pop(msg);
    char *id_string = zframe_strhex(sender);
    int worker_exist = (this->InquireWorker(id_string) != NULL);
    free (id_string);
    Worker *worker = this->RequireWorker(sender);

    // worker通知broker已准备就绪
    if (zframe_streq(command, MDPW_READY)) {
        if (worker_exist) {
            worker->Delete(1);
        } else {
            if (zframe_size(sender) >= 4
                && !memcmp(zframe_data(sender), "mmi.", 4)) {
                worker->Delete(1);
            } else {
                zframe_t *servicename = zmsg_pop(msg);
                worker->service_ = this->RequireService(servicename);
                worker->Waiting();
                zframe_destroy(&servicename);
            }
        }
    } else if (zframe_streq(command, MDPW_REPLY)) {
        if (worker_exist) {
            zframe_t *client = zmsg_unwrap(msg);
            zmsg_pushstr(msg, worker->service_->name_);
            zmsg_pushstr(msg, MDPC_CLIENT);
            zmsg_wrap(msg, client);
            zmsg_send(&msg, this->socket_);
            worker->Waiting();
        } else {
            worker->Delete(1);
        }
    } else if (zframe_streq(command, MDPW_HEARTBEAT)) {
        if (worker_exist) {
            worker->expiry_ = zclock_time() + HEARTBEAT_EXPIRY;
        } else {
            worker->Delete(1);
        }
    } else if (zframe_streq(command, MDPW_DISCONNECT)) {
        worker->Delete(0);
    } else {
        zclock_log("E: invalid input message");
        zmsg_dump(msg);
    }

    zframe_destroy(&command);
    zmsg_destroy(&msg);
}

// 处理客户端消息 
void Broker::ProcessClientMsg (zframe_t *sender, zmsg_t *msg)
{
    assert (zmsg_size(msg) >= 2);  // service name + body

    zframe_t *servicename = zmsg_pop(msg);

    zmsg_wrap(msg, zframe_dup(sender));

    // 客户端的服务查询请求
    if (zframe_size(servicename) >= 4 && 
        !memcmp(zframe_data(servicename), "mmi.", 4)) {
        ServiceInternal(servicename, msg);
    // 客户端需要转发到后端服务处理的请求
    } else {
        this->DispatchMsg(servicename, msg);
        zframe_destroy(&servicename);
    }
}
// 清除broker中过期的worker
void Broker::Purge()
{
    Worker *worker = this->FirstWaitWorker();
    while (worker) {
        if (zclock_time() < worker->expiry_)
            break;
        if (this->verbose_) {
            zclock_log("I: deleting expired worker: %s", worker->id_string_);
        }
        worker->Delete(0);
        worker = this->FirstWaitWorker();
    }
}

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));
    Broker *broker = new Broker(verbose);
    int ret = broker->Bind(BROKER_ADDR);
    assert(ret != -1);

    // 接收clients和workers的消息并处理
    while (1)
    {
        zmq_pollitem_t items[] = 
        {
            { broker->socket_, 0, ZMQ_POLLIN, 0 }
        };
        ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), HEARTBEAT_INTERVAL);
        if (ret == -1) break;

        if (items[0].revents & ZMQ_POLLIN)
        {
            zmsg_t *msg = zmsg_recv(broker->socket_);
            if (!msg) break;
            if (broker->verbose_)
            {
                zclock_log ("I: received message:");
                zmsg_dump(msg);
            }
            zframe_t *sender = zmsg_pop(msg);
            zframe_t *empty = zmsg_pop(msg);
            zframe_t *header = zmsg_pop(msg);

            if (zframe_streq(header, MDPC_CLIENT))
            {
                broker->ProcessClientMsg(sender, msg);
            }
            else if (zframe_streq(header, MDPW_WORKER))
            {
                broker->ProcessWorkerMsg(sender, msg);
            }
            else 
            {
                zclock_log("E: invalid message:");
                zmsg_dump(msg);
                zmsg_destroy(&msg);
            }

            zframe_destroy(&sender);
            zframe_destroy(&empty);
            zframe_destroy(&header);
        }

        if (zclock_time() > broker->heartbeat_at_)
        {
            // 断开并删除过期的worker
            broker->Purge();
            // 给所有有效的worker发送心跳包
            Worker *worker = broker->FirstWaitWorker();
            while (worker)
            {
                worker->Send(MDPW_HEARTBEAT, NULL, NULL);
                worker = broker->NextWaitWorker();
            }
            broker->heartbeat_at_ = zclock_time() + HEARTBEAT_INTERVAL;
        }
    }
    delete broker;
    return 0;
}
