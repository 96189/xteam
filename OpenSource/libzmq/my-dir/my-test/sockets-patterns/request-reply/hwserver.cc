
// 请求-响应

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    // 准备上下文
    void *context = zmq_init(1);

    // 准备REP套接字 并 绑定套接字到某地址
    void *responder = zmq_socket(context, ZMQ_REP);
    int ret = zmq_bind(responder, "tcp://*:5555");
    assert(ret != -1);

    while (1)
    {
        // 接收请求
        zmq_msg_t request;
        zmq_msg_init(&request);
        int n = zmq_msg_recv(&request, responder, 0);
        assert(n > 0);
        printf("recv req : %s \n", (char*)zmq_msg_data(&request));
        zmq_msg_close(&request);

        // 响应
        zmq_msg_t reply;
        char buf[256] = { '\0' };
        memcpy(buf, "world", 5);
        zmq_msg_init_data(&reply, buf, strlen(buf) + 1, NULL, NULL);
        n = zmq_msg_send(&reply, responder, 0);
        assert(n > 0);
        zmq_msg_close(&reply);
    }

    zmq_close(responder);
    zmq_term(context);
    return 0;
}