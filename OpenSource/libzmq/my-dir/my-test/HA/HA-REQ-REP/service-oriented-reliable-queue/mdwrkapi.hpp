#ifndef _MD_WORK_HPP_
#define _MD_WORK_HPP_
//

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string.h>

#include "mdp.h"

#define DEFAULT_LIVENESS 3
#define DEFAULE_HEARTBEAT 2500  // ms
#define DEFAULT_RECONNECT 2500  // ms

class MdWork 
{
private:
    void *context_;         // zmq context
    char *broker_;          // broker地址
    char *service_;         // 服务名称
    void *worker_;          // worker套接字
    int verbose_;           

    int64_t heartbeat_at_;  // 心跳开始时间
    size_t liveness_;       // 心跳失败允许的次数
    int heartbeat_;         // 心跳间隔
    int reconnect_;         // 重连次数

    int expect_reply_;
    zframe_t *reply_to_;

public:
// 构造 析构
    MdWork(const char *broker, const char* service, int verbose, 
           int liveness=DEFAULT_LIVENESS, int heartbeat=DEFAULE_HEARTBEAT, int reconnect=DEFAULT_RECONNECT) 
        : broker_(strdup(broker)), service_(strdup(service)), worker_(NULL), verbose_(verbose), heartbeat_at_(0), 
          liveness_(liveness), heartbeat_(heartbeat), reconnect_(reconnect), expect_reply_(0), reply_to_(NULL)
    {
        context_ = zmq_init(1);
    }
    ~MdWork()
    {
        if (worker_) zmq_close(worker_);
        if (service_) free(service_);
        if (broker_) free(broker_);
        if (context_) zmq_term(context_);
    }
//
    int Connect2Broker()
    {
        if (this->worker_)
            zmq_close(this->worker_);
        this->worker_ = zmq_socket(this->context_, ZMQ_DEALER);
        int ret = zmq_connect(this->worker_, this->broker_);
        if (this->verbose_)
            zclock_log ("I: connecting to broker at %s…", this->broker_);

        this->Send(MDPW_READY, this->service_, NULL);

        this->liveness_ = DEFAULT_LIVENESS;
        this->heartbeat_at_ = zclock_time() + this->heartbeat_at_;
        return ret;
    }
    void Send(char *command, char *option, zmsg_t *msg)
    {
        msg = msg ? zmsg_dup(msg) : zmsg_new();
        if (option)
            zmsg_pushstr(msg, option);
        zmsg_pushstr(msg, command);
        zmsg_pushstr(msg, MDPW_WORKER);
        zmsg_pushstr(msg, "");

        if (this->verbose_)
        {
            zclock_log ("I: sending %s to broker",mdps_commands [(int) *command]);
            zmsg_dump (msg);           
        }
        zmsg_send(&msg, this->worker_);
    }
    void SetHeartbeat(int heartbeat)
    {
        this->heartbeat_ = heartbeat;
    }
    void SetReconnect(int reconnect)
    {
        this->reconnect_ = reconnect;
    }
    zmsg_t *Recv(zmsg_t **reply_p)
    {
        zmsg_t *reply = *reply_p;
        assert(reply || !this->expect_reply_);
        if (reply)
        {
            assert(this->reply_to_);
            zmsg_wrap(reply, this->reply_to_);
            this->Send(MDPW_REPLY, NULL, reply);
            zmsg_destroy(reply_p);
        }

        this->expect_reply_ = 1;

        while (1)
        {
            zmq_pollitem_t items[] = 
            {
                { this->worker_, 0, ZMQ_POLLIN, 0 }
            };
            int ret = zmq_poll(items, sizeof(items)/sizeof(items[0]), this->heartbeat_);
            if (ret == -1) break;

            if (items[0].revents & ZMQ_POLLIN)
            {
                zmsg_t *msg = zmsg_recv(this->worker_);
                if (!msg) break;
                if (this->verbose_)
                {
                    zclock_log ("I: received message from broker:");
                    zmsg_dump (msg);
                }
                this->liveness_ = DEFAULT_LIVENESS;
                assert(zmsg_size(msg) >= 3);

                zframe_t *empty = zmsg_pop(msg);
                assert(zframe_streq(empty, ""));
                zframe_destroy(&empty);

                zframe_t *header = zmsg_pop(msg);
                assert(zframe_streq(header, MDPW_WORKER));
                zframe_destroy(&header);

                zframe_t *command = zmsg_pop(msg);
                if (zframe_streq(command, MDPW_REQUEST))
                {
                    this->reply_to_ = zmsg_unwrap(msg);
                    zframe_destroy(&command);
                    return msg;     // success
                }
                else if (zframe_streq(command, MDPW_HEARTBEAT))
                {
                    // nothing
                }
                else if (zframe_streq(command, MDPW_DISCONNECT))
                {
                    this->Connect2Broker();
                }
                else 
                {
                    zclock_log ("E: invalid input message");
                    zmsg_dump(msg);
                }
                zframe_destroy(&command);
                zmsg_destroy(&msg);
            }
            // 超时
            else 
            {
                if (--this->liveness_ == 0)
                {
                    if (this->verbose_)
                        zclock_log ("W: disconnected from broker - retrying…");
                    zclock_sleep(this->reconnect_);

                    this->Connect2Broker();
                }
            }

            if (zclock_time () > this->heartbeat_at_) 
            {
                this->Send (MDPW_HEARTBEAT, NULL, NULL);

                this->heartbeat_at_ = zclock_time () + this->heartbeat_;
            }
        }
        return NULL;    // failiure
    }
};

#endif