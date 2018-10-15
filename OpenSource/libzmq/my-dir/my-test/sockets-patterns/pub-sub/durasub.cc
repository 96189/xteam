
// 持久化的订阅者

#include <zmq.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // 创建上下文信息
    void *context = zmq_init(1);

    // 创建订阅套接字 订阅所有消息 告知发布端缓存本订阅端的消息 发起连接
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);
    zmq_setsockopt(subscriber, ZMQ_IDENTITY, "Hello", 5);   // 为套接字设置标识
    int ret = zmq_connect(subscriber, "tcp://localhost:5565");
    assert(ret != -1);

    int n = -1;
    // 创建同步套接字 发送同步消息
    void *sync = zmq_socket(context, ZMQ_PUSH);
    ret = zmq_connect(sync, "tcp://localhost:5564");
    assert(ret != -1);
    n = zmq_send(sync, "ready", 6, 0);
    assert(n > 0);

    // 在订阅套接字上接收消息
    char buf[256] = {'\0'};
    while (1)
    {
        n = zmq_recv(subscriber, buf, 255, 0);
        assert(n > 0);
        printf("%s\n", buf);
        if (!strncmp(buf,"END",3))
        {
            break;
        }
    }
    zmq_close(subscriber);
    zmq_close(sync);
    zmq_term(context);
    return 0;
}