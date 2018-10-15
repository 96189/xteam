
// 发布-订阅消息信封 订阅者

#include <zmq.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    // 初始化上下文
    void *context = zmq_init(1);

    // 订阅套接字连接发布套接字
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int ret = zmq_connect(subscriber, "tcp://localhost:5563");
    assert(ret != -1);
    // 只订阅B类型的信封
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "B", 1);  
    
    int n = -1;
    while (1)
    {
        char envelope[64] = {'\0'};
        char message[256] = {'\0'};
        n = zmq_recv(subscriber, envelope, 63, 0);
        assert(n > 0);
        n = zmq_recv(subscriber, message, 255, 0);
        printf("[%s] %s\n", envelope, message);
    }
    zmq_close(subscriber);
    zmq_term(context);
    return 0;
}