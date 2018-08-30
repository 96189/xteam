
// 气象信息代理服务装置

#include <zmq.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define PRIVATE_ADDR "tcp://172.16.0.11:5556"
#define PUBLIC_ADDR "tcp://*:8100"

int main(int argc, char* argv[])
{
    void *context = zmq_init(1);

    // 订阅气象信息
    void *frontend = zmq_socket(context, ZMQ_SUB);
    int ret = zmq_connect(frontend, PRIVATE_ADDR);
    assert(ret != -1);

    // 转发气象信息
    void *backend = zmq_socket(context, ZMQ_PUB);
    ret = zmq_bind (backend, PUBLIC_ADDR);
    assert(ret != -1);

    // 订阅所有消息
    zmq_setsockopt(frontend, ZMQ_SUBSCRIBE, "", 0);
    
    // 转发消息
    while (1)
    {
        while (1)
        {
            zmq_msg_t message;
            int64_t more;

            // 处理所有消息的帧
            zmq_msg_init(&message);
            int n = zmq_msg_recv(&message, frontend, 0);
            assert(n > 0);
            size_t more_size = sizeof(more);
            zmq_getsockopt(frontend, ZMQ_RCVMORE, &more, &more_size);
            n = zmq_msg_send(&message, backend, more ? ZMQ_SNDMORE : 0);
            assert(n > 0);
            zmq_close(&message);
            if (!more)
                break;  // 已达到最后一帧
        }
    }

    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}