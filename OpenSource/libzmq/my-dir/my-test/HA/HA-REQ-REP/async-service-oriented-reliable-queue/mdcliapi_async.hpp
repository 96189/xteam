
//

#ifndef _MD_CLI_API_ASYNC_HPP_ 
#define _MD_CLI_API_ASYNC_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string.h>

#include "mdp.h"

#define DEFAULT_TIMEOUT 2500
#define DEFAULT_RETRIES 3

class MdCliAsync
{
private:
    void *context_;     // zmq context
    char *broker_;      // broker地址
    void *client_;      // 客户端连接套接字
    int verbose_;
    int timeout_;

public:
// 构造 析构
    MdCliAsync(const char *broker, int verbose, int timeout = DEFAULT_TIMEOUT, int retries = DEFAULT_RETRIES)
        : broker_(strdup(broker)), client_(NULL), verbose_(verbose), timeout_(timeout)
    {
        context_ = zmq_init(1);
    }
    ~MdCliAsync()
    {
        if (broker_)
            free(broker_);
        if (client_)
            zmq_close(client_);
        zmq_term(context_);
    }
// 
    int Connect2Broker()
    {
        if (this->client_)
            zmq_close(this->client_);
        this->client_ = zmq_socket(context_, ZMQ_DEALER);   // 异步套接字
        int ret = zmq_connect(this->client_, this->broker_);
        if (this->verbose_)
            zclock_log("I: connecting to broker at %s…", this->broker_);
        return ret;
    }
    void SetTimeOut(int timeout)
    {
        this->timeout_ = timeout;
    }
    int Send(char *service, zmsg_t **request_p)
    {
        assert(request_p);
        zmsg_t *request = *request_p;

        zmsg_pushstr(request, service);
        zmsg_pushstr(request, MDPC_CLIENT);
        zmsg_pushstr(request, "");      // ZMQ_DEALER
        if (this->verbose_)
        {
            zclock_log("I: send request to '%s' service:", service);
            zmsg_dump(request);
        }
        zmsg_send(&request, this->client_);
        return 0;
    }

    zmsg_t *Recv()
    {
        zmq_pollitem_t items[] = {
            {this->client_, 0, ZMQ_POLLIN, 0}
        };
        int ret = zmq_poll(items, sizeof(items) / sizeof(items[0]), this->timeout_);
        if (ret == -1) return NULL;

        if (items[0].revents & ZMQ_POLLIN) {
            zmsg_t *msg = zmsg_recv(this->client_);
            if (this->verbose_) {
                zclock_log("I: received reply:");
                zmsg_dump(msg);
            }
            assert(zmsg_size(msg) >= 4);

            zframe_t *empty = zmsg_pop(msg);
            assert(zframe_streq(empty, ""));
            zframe_destroy(&empty);

            zframe_t *header = zmsg_pop(msg);
            assert(zframe_streq(header, MDPC_CLIENT));
            zframe_destroy(&header);

            zframe_t *service = zmsg_pop(msg);
            zframe_destroy(&service);

            return msg; // success
        }
        // 超时
        else {
            if (zctx_interrupted) {
                printf("W: interrupt received, killing client…\n");
                return NULL;
            }
            if (this->verbose_) {
                zclock_log("W: permanent error, abandoning");
            }
        }
        return NULL;
    }
};

#endif