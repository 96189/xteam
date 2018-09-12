
//

#ifndef _MD_CLI_API_HPP_ 
#define _MD_CLI_API_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string.h>

#include "mdp.h"

#define DEFAULT_TIMEOUT 2500
#define DEFAULT_RETRIES 3

class MdCli
{
private:
    void *context_;     // zmq context
    char *broker_;      // broker地址
    void *client_;      // 客户端连接套接字
    int verbose_;
    int timeout_;
    int retries_;

public:
// 构造 析构
    MdCli(const char *broker, int verbose, int timeout = DEFAULT_TIMEOUT, int retries = DEFAULT_RETRIES)
        : broker_(strdup(broker)), client_(NULL), verbose_(verbose), timeout_(timeout), retries_(retries)
    {
        context_ = zmq_init(1);
    }
    ~MdCli()
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
        this->client_ = zmq_socket(context_, ZMQ_REQ);
        int ret = zmq_connect(this->client_, this->broker_);
        if (this->verbose_)
            zclock_log("I: connecting to broker at %s…", this->broker_);
        return ret;
    }
    void SetTimeOut(int timeout)
    {
        this->timeout_ = timeout;
    }
    void SetRetries(int retries)
    {
        this->retries_ = retries;
    }
    zmsg_t *Send(char *service, zmsg_t **request_p)
    {
        assert(request_p);
        zmsg_t *request = *request_p;

        zmsg_pushstr(request, service);
        zmsg_pushstr(request, MDPC_CLIENT);
        if (this->verbose_)
        {
            zclock_log ("I: send request to '%s' service:", service);
            zmsg_dump (request);
        }
        int retires_left = this->retries_;
        while (retires_left > 0)
        {
            zmsg_t *msg = zmsg_dup(request);
            zmsg_send(&msg, this->client_);

            zmq_pollitem_t items[] = 
            {
                { this->client_, 0, ZMQ_POLLIN, 0 }
            };
            int ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), this->timeout_);
            if (ret == -1) break;

            if (items[0].revents & ZMQ_POLLIN)
            {
                zmsg_t *msg = zmsg_recv(this->client_);
                if (this->verbose_)
                {
                    zclock_log ("I: received reply:");
                    zmsg_dump(msg);
                }
                assert(zmsg_size(msg) >= 3);
                zframe_t *header = zmsg_pop(msg);
                assert(zframe_streq(header, MDPC_CLIENT));
                zframe_destroy(&header);

                zframe_t *reply = zmsg_pop(msg);
                assert(zframe_streq(reply, service));
                zframe_destroy(&reply);

                zmsg_destroy(&request);
                return msg;     // success
            }
            // 超时
            else 
            {
                if (--retires_left > 0)
                {
                    if (this->verbose_)
                        zclock_log ("W: no reply, reconnecting…");
                    this->Connect2Broker();
                }
                else 
                {
                    if (this->verbose_)
                        zclock_log ("W: permanent error, abandoning");
                    break;
                }
            }
        }
        zmsg_destroy(&request);
        return NULL;    // failiure
    }
};

#endif