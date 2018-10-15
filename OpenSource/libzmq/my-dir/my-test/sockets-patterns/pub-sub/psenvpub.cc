
// 发布-订阅消息信封 发布者

#include <zmq.h>
#include <assert.h>
#include <unistd.h>
#include <string>

int main(int argc, char* argv[])
{
    // 创建上下文
    void *context = zmq_init(1);

    // 发布套接字和地址绑定
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int ret = zmq_bind(publisher, "tcp://*:5563");
    assert(ret != -1);

    int n = -1;
    while (1)
    {
        // 信封A 消息
        std::string msg_A = "We don't want to see this";
        n = zmq_send(publisher, "A", 1, ZMQ_SNDMORE);
        assert(n > 0);
        n = zmq_send(publisher, msg_A.c_str(), msg_A.size(), 0);
        assert(n > 0);

        // 信封B 消息
        std::string msg_B = "We would like to see this";
        n = zmq_send(publisher, "B", 1, ZMQ_SNDMORE);
        assert(n > 0);
        n = zmq_send(publisher, msg_B.c_str(), msg_B.size(), 0);
        assert(n > 0);

        sleep(1);
    }
    zmq_close(publisher);
    zmq_term(context);
    return 0;
}