
// 订阅者 - 同步版

#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    void *context = zmq_init(1);

    // 订阅套接字 并订阅所有消息
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    int ret = zmq_connect(subscriber, "tcp://localhost:5561");
    assert(ret != -1);
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "", 0);

    sleep(1);

    // 同步套接字
    void *syncclient = zmq_socket(context, ZMQ_REQ);
    ret = zmq_connect(syncclient, "tcp://localhost:5562");
    assert(ret != -1);

    // 同步套接字上通知远程本端已准备好
    int n = zmq_send(syncclient, "ready", 6, 0);
    assert(n > 0);

    // 同步套接字接收远程的回应
    char buf[256] = {'\0'};
    n = zmq_recv(syncclient, buf, 255, 0);

    // 订阅套接字上收消息
    int update_nbr;
    while (1)
    {
        memset(buf, 0, 256);
        n = zmq_recv(subscriber, buf, 255, 0);
        assert(n >= 0);
        if (!strncmp(buf, "END", 3))
        {
            break;
        }
        update_nbr++;
    }
    printf("[%d] recv %d message\n", getpid(), update_nbr);

    zmq_close(subscriber);
    zmq_close(syncclient);
    zmq_term(context);
    return 0;
}