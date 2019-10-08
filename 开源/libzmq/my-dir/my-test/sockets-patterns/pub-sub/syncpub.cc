
// 发布者 - 同步版

#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <string>

// 等待n个订阅者连接
#define SUBSCRIBERS_EXPECTED 10
#define MESSAGE_COUNT 999

int main(int argc, char *argv[])
{
    void *context = zmq_init(1);

    // 发布套接字
    void *publisher = zmq_socket(context, ZMQ_PUB);
    int ret = zmq_bind(publisher, "tcp://*:5561");
    assert(ret != -1);

    // 同步套接字
    void *syncservice = zmq_socket(context, ZMQ_REP);
    ret = zmq_bind(syncservice, "tcp://*:5562");
    assert(ret != -1);

    char buf[256] = {'\0'};
    int n = -1;

    // 在同步套接字上接收订阅者准备信号 回复消息给远程订阅者 并统计已准备接收的订阅者数量
    for (int subscribers = 0; subscribers < SUBSCRIBERS_EXPECTED; ++subscribers)
    {
        n = zmq_recv(syncservice, buf, 255, 0);
        assert(n >= 0);
        n = zmq_send(syncservice, "wait", 5, 0);
        assert(n > 0);
    }

    // 在发布套接字上广播消息给订阅者
    for (int update_nbr = 0; update_nbr < MESSAGE_COUNT; ++update_nbr)
    {
        std::string message = "message info *****";
        memset(buf, 0, 256);
        memcpy(buf, message.c_str(), strlen(message.c_str()));
        n = zmq_send(publisher, buf, 256, 0);
        assert(n > 0);
    }

    // 发布套接字上通知远程订阅端消息发布完毕
    n = zmq_send(publisher, "END", 4, 0);
    assert(n > 0);

    zmq_close(publisher);
    zmq_close(syncservice);
    zmq_term(context);
    return 0;
}