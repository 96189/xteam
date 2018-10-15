
// 请求-应答代理

#include <zmq.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    // 创建上下文
    void *context = zmq_init(1);

    // 创建ROUTER端点和DEALER套接字 并绑定到某地址
    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    void *backend = zmq_socket(context, ZMQ_DEALER);
    int ret = zmq_bind(frontend, "tcp://*:5559");
    assert(ret != -1);
    ret = zmq_bind(backend, "tcp://*:5560");
    assert(ret != -1);

    // 初始化轮询集合
    zmq_pollitem_t items[] = 
    {
        { frontend, 0, ZMQ_POLLIN, 0 },
        { backend, 0, ZMQ_POLLIN, 0 }
    };

    // 在套接字间转发消息
    while (1)
    {
        zmq_msg_t message;
        int64_t more;   // 检测多帧消息

        ret = zmq_poll(items, 2, -1);
        assert(ret != -1);

        if (items[0].revents & ZMQ_POLLIN)
        {
            // 处理所有消息帧 frontend -> backend
            while (1)
            {
                zmq_msg_init(&message);
                int n = zmq_msg_recv(&message, frontend, 0);
                assert(n >= 0);
                size_t more_size = sizeof(more);
                zmq_getsockopt(frontend, ZMQ_RCVMORE, &more, &more_size);
                n = zmq_msg_send(&message, backend, more ? ZMQ_SNDMORE : 0);
                assert(n >= 0);
                zmq_msg_close(&message);
                if (!more)
                    break;  // 最后一帧
            }
        }

        if (items[1].revents & ZMQ_POLLIN)
        {
            // 处理所有消息帧 backend -> frontend 
            while (1)
            {
                zmq_msg_init(&message);
                int n = zmq_msg_recv(&message, backend, 0);
                assert(n >= 0);
                size_t more_size = sizeof(more);
                zmq_getsockopt(backend, ZMQ_RCVMORE, &more, &more_size);
                n = zmq_msg_send(&message, frontend, more ? ZMQ_SNDMORE : 0);
                assert(n >= 0);
                zmq_msg_close(&message);
                if (!more)
                    break;  // 最后一帧
            }
        }
    }

    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}