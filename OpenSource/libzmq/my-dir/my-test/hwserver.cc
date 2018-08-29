
// 请求-响应

#include <zmq.h>
#include <iostream>
#include <string.h>

int main(int argc, char* argv[])
{
    void *context = zmp_init(1);

    void *responder = zmp_socket(context, ZMQ_REP);
    zmq_bind(responder, "tcp://*:5555");

    while (1)
    {
        zmq_msg_t request;
        zmq_msg_init(&request);
        zmq_recv(responder, &request, 0);
        std::cout << request << std::endl;
        zmq_msg_close(&request);


        zmq_msg_t reply;
        zmq_msg_init_size(&reply, 5);
        memcpy(zmq_msg_data(&reply), "world", 5);
        zmq_send(responder, &reply, 0);
        zmq_msg_close(&reply);
    }

    zmq_close(responder);
    zmq_term(context);
    return 0;
}