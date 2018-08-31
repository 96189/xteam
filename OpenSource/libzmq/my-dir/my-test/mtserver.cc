
// 多线程版hello world服务

#include <zmq.h>
#include <pthread.h>
#include <assert.h>

#define THREAD_NUM 5

// 后端线程任务处理函数
static void *worker_routine(void *context)
{
    // 创建具体的响应套接字 且向后端的对外接口发起连接
    void *recevier = zmq_socket(context, ZMQ_REP);
    int ret = zmq_connect(recevier, "inproc://workers");
    assert(ret != -1);

    // 循环收发消息
    while (1)
    {
        char buf[256] = {'\0'};
        int n = zmq_recv(recevier, buf, 255, 0);
        assert(n >= 0);
        printf("server %lu thread recv : %s\n", pthread_self(), buf);
        n = zmq_send(recevier, "world", 6, 0);
        assert(n >= 0);
    }
    zmq_close(recevier);
    return NULL;
}

int main(int argc, char* argv[])
{
    // 创建上下文
    void *context = zmq_init(1);

    // 创建前端后端套接字 并按各自协议绑定相关地址
    void *clients = zmq_socket(context, ZMQ_ROUTER);
    void *workers = zmq_socket(context, ZMQ_DEALER);
    int ret = zmq_bind(clients, "tcp://*:5555");
    assert(ret != -1);
    ret = zmq_bind(workers, "inproc://workers");
    assert(ret != -1);

    // 后端线程池创建
    for (int thread_nbr = 0; thread_nbr < THREAD_NUM; ++thread_nbr)
    {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_routine, context);
    }

    // 前后端套接字组合为queue 方便消息中转
    ret = zmq_device(ZMQ_QUEUE, clients, workers);
    assert(ret != -1);

    zmq_close(clients);
    zmq_close(workers);
    zmq_term(context);
    return 0;
}