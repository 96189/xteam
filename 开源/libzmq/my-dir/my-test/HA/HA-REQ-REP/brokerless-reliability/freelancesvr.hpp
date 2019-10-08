//

#ifndef _FREE_LANCE_SVR_HPP_
#define _FREE_LANCE_SVR_HPP_

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <string>

class FreeSvr 
{
private:
    void *context_;
    void *server_;
    std::string endpoint_;
    std::string identity_;

public:
// 构造 析构
    FreeSvr()
    {
        context_ = zmq_init(1);
        server_ = NULL;
    }
    ~FreeSvr()
    {
        zmq_close(server_);
        zmq_term(context_);
    }
// 绑定端点
    int Bind(std::string endpoint, std::string identity)
    {
        endpoint_ = endpoint;
        identity_ = identity;
        server_ = zmq_socket(context_, ZMQ_ROUTER);
        zmq_setsockopt(server_, ZMQ_IDENTITY, (char*)identity.c_str(), identity.size());
        int ret = zmq_bind(server_, (char*)endpoint_.c_str());
        assert(ret != -1);
        printf("I: service is ready at %s\n", endpoint_.c_str());
    }
//
    void Loop(int verbose)
    {
        while (!zctx_interrupted)
        {
            zmsg_t *request = zmsg_recv(server_);
            if (verbose && request)
            {
                zmsg_dump(request);
            }
            if (!request) break;

            zframe_t *identity = zmsg_pop(request);
            zframe_t *control = zmsg_pop(request);
            zmsg_t *reply = zmsg_new();
            if (zframe_streq(control, "PING"))
            {
                zmsg_addstr(reply, "PONG");
            }
            else 
            {
                zmsg_add(reply, control);
                zmsg_addstr(reply, "OK");
            }
            zmsg_destroy(&request);
            zmsg_prepend(reply, &identity);
            if (verbose && reply)
            {
                zmsg_dump(reply);
            }
            zmsg_send(&reply, this->server_);
        }
        printf("W: interrupted\n");
    }
};

#endif