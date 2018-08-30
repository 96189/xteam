
// 连接REP套接字至tcp://*:5560端点
// 接收hello请求 返回world应答

#include <zmq.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    void *context = zmq_init(1);

    void *responder = zmq_socket(context, ZMQ_REP);
    int ret = zmq_connect(responder, "tcp://localhost:5560");
    assert(ret != -1);

    while (1)
    {
        // recv request
        char buf[256] = {'\0'};
        int n = zmq_recv(responder, buf, 255, 0);
        assert(n > 0);
        printf("recv request : %s\n", buf);

        // do somethin

        // response
        n = zmq_send(responder, "world", 5, 0);
        assert(n > 0);
    }

    zmq_close(responder);
    zmq_term(context);
    return 0;
}