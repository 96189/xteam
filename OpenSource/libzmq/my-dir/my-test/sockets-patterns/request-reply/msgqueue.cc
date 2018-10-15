
// 请求-应答代理broker(使用zmq内置的queue)

#include <zmq.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    void *context = zmq_init(1);

    // ROUTER端套接字
    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    int ret = zmq_bind(frontend, "tcp://*:5559");
    assert(ret != -1);

    // DEALER端套接字
    void *backend = zmq_socket(context, ZMQ_DEALER);
    ret = zmq_bind(backend, "tcp://*:5560");
    assert(ret != -1);

    // 使用内置queue关联frontend和backend以便转发消息
    ret = zmq_device(ZMQ_QUEUE, frontend, backend);
    assert(ret != -1);

    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}