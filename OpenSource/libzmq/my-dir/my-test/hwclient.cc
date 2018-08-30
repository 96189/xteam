
// 请求-响应

#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char* argv[])
{
    // 准备上下文
    void *context = zmq_init(1);

    // 准备REQ套接字 并 连接服务器
    void *requester = zmq_socket(context, ZMQ_REQ);
    int ret = zmq_connect(requester, "tcp://localhost:5555");
    assert(ret != -1);
    
    for (int request_nbr = 0; request_nbr != 10; ++request_nbr)
    {
        // 发起请求
        zmq_msg_t request;
        char buf[256] = { '\0' };
        memcpy(buf, "hello", 5);
        zmq_msg_init_data(&request, buf, strlen(buf) + 1, NULL, NULL);
        int n = zmq_msg_send(&request, requester, 0);
        assert(n > 0);
        zmq_msg_close(&request);    
        
        // 接收响应
        zmq_msg_t response;
        ret =  zmq_msg_init(&response);
        n = zmq_msg_recv(&response, requester, 0);
        assert(n > 0);
        printf("recv res : %s \n", (char*)zmq_msg_data(&response));
        zmq_msg_close(&response);
    }
    zmq_close(requester);
    zmq_term(context);
    return 0;
}