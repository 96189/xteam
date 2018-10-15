
// 发布者 - 连接持久化订阅者

#include <zmq.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#define MESSAGE_COUNT 10
#define SAVE_DISCONNET_MESSAGE_COUNT 2

int main(int argc, char *argv[])
{
    // 创建上下文
    void *context = zmq_init(1);

    // 创建发布套接字 并对消息做持久化设置 绑定到具体地址
    void *publisher = zmq_socket(context, ZMQ_PUB);
    uint64_t hwm = SAVE_DISCONNET_MESSAGE_COUNT;
    zmq_setsockopt(publisher, ZMQ_SNDHWM, &hwm, sizeof(hwm));
    // ZMQ_SWAP新版中已经废弃
    // uint64_t swap = 25000000;
    // zmq_setsockopt(publisher, ZMQ_SWAP, &swap, sizeof(swap));
    int ret = zmq_bind(publisher, "tcp://*:5565");
    assert(ret != -1);

    // 创建同步套接字 并绑定到具体地址
    void *sync = zmq_socket(context, ZMQ_PULL);
    ret = zmq_bind(sync, "tcp://*:5564");
    assert(ret != -1);

    char buf[256] = {'\0'};
    int n = -1;
    // 在同步套接字上接收订阅端发来的同步消息
    n = zmq_recv(sync, buf, 255, 0);
    assert(n > 0);

    // 在发布套接字上循环发送消息给订阅端
    // 发送n条消息 1秒1条
    for (int update_nbr = 0; update_nbr < MESSAGE_COUNT; ++update_nbr)
    {
        memset(buf, 0, 256);
        snprintf(buf, 256, "Update %d", update_nbr);
        n = zmq_send(publisher, buf, strlen(buf) + 1, 0);
        assert(n > 0);
        sleep(1);
    }
    // 在发布套接字上通知所有的订阅端消息发布结束
    n = zmq_send(publisher, "END", 4, 0);
    assert(n > 0);

    zmq_close(publisher);
    zmq_close(sync);
    zmq_term(context);
    return 0;
}