
// 连接REQ套接字至tcp://localhost:5559端点(代理)
// 发送hello给服务端 等待world应答

#include <zmq.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    void *context = zmq_init(1);

    void *requester = zmq_socket(context, ZMQ_REQ);
    int ret = zmq_connect(requester, "tcp://localhost:5559");
    assert(ret != -1);

    for (int request_nbr = 0; request_nbr < 10; ++request_nbr)
    {
        // request
        int n = zmq_send(requester, "hello", 5, 0);
        assert(n > 0);
        
        // recv response
        char buf[256] = {'\0'};
        n = zmq_recv(requester, buf, 255, 0);
        assert(n > 0);
        printf("recv response : %s\n", buf);
    }

    zmq_close(requester);
    zmq_term(context);
    return 0;
}