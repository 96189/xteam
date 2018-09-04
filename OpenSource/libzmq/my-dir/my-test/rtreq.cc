
// 自定义 ROUTER-REQ请求

#include <zmq.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>

#define NBR_WORKERS 10
#define BUFF_SIZE 64

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

void *worker_task(void *args)
{
    void *context = zmq_init(1);

    void *worker = zmq_socket(context, ZMQ_REQ);
    assert(worker);

    // 设置套接字标识 setsockopt ZMQ_IDENTITY
    // 示例用 现实中REQ套接字都是匿名的
    char identity [10];
    sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
    zmq_setsockopt (worker, ZMQ_IDENTITY, identity, strlen (identity));

    // REQ套接字连接ROUTER套接字
    int ret = zmq_connect(worker, "ipc://routing.ipc");
    assert(ret != -1);

    int total = 0;
    int n = -1;
    char msg[BUFF_SIZE] = {'\0'};
    while (1)
    {
        memset(msg, 0, BUFF_SIZE);
        memcpy(msg, "ready", 5);
        // 通知ROUTER 当前worker已准备
        n = zmq_send(worker, msg, BUFF_SIZE, 0);
        assert(n > 0);
        // 从ROUTER获取任务 直到收到结束消息
        memset(msg, 0, BUFF_SIZE);
        n = zmq_recv(worker, msg, BUFF_SIZE, 0);
        assert(n >= 0);
        if (!strncmp(msg, "END", 3))
        {
            printf("Processed: %d tasks\n", total);
            break;
        }
        ++total;

        // 随机等待一段时间
        usleep(randof(1000) + 1);
    }

    zmq_close(worker);
    zmq_term(context);
    return NULL;
}

int main(int argc, char *argv[])
{
    void *context = zmq_init(1);

    // broker在ROUTER套接字 处理n个REQ的worker
    void *broker = zmq_socket(context, ZMQ_ROUTER);
    int ret = zmq_bind(broker, "ipc://routing.ipc");
    assert(ret != -1);

    // 启动多线程 并执行线程函数
    // 每个线程为一个worker 且为REQ套接字 连接ROUTER套接字
    int worker_nbr;
    for (worker_nbr = 0; worker_nbr < NBR_WORKERS; ++worker_nbr)
    {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_task, NULL);
    }

    // 任务分发 最近最少使用的worker在消息队列中 LRU负载均衡
    int n = -1;
    char identify[BUFF_SIZE];
    char delimiter[BUFF_SIZE];
    char data[BUFF_SIZE];
    for (int task_nbr = 0; task_nbr < NBR_WORKERS * 10; ++task_nbr)
    {
        memset(identify, 0, BUFF_SIZE);
        memset(delimiter, 0, BUFF_SIZE);
        memset(data, 0, BUFF_SIZE);
        n = zmq_recv(broker, identify, BUFF_SIZE, 0);
        assert(n >= 0);
        n = zmq_recv(broker, delimiter, BUFF_SIZE, 0);
        assert(n >= 0);
        n = zmq_recv(broker, data, BUFF_SIZE, 0);
        assert(n >= 0);

        if (!strncmp(data, "ready", 5))
        {
            std::string message = "This is the workload";
            char send_data[BUFF_SIZE];
            memcpy(send_data, message.c_str(), message.size());

            n = zmq_send(broker, identify ,strlen(identify), ZMQ_SNDMORE);
            assert(n > 0);
            n = zmq_send(broker, "", 0, ZMQ_SNDMORE);
            assert(n >= 0);
            n = zmq_send(broker, send_data, strlen(send_data), 0);
            assert(n > 0);
        }

    }

    // 通知所有套接字结束工作
    for (worker_nbr = 0; worker_nbr < NBR_WORKERS; ++worker_nbr)
    {
        n = zmq_recv(broker, identify, BUFF_SIZE, 0);
        assert(n >= 0);
        n = zmq_recv(broker, delimiter ,BUFF_SIZE, 0);
        assert(n >= 0);
        n = zmq_recv(broker, data , BUFF_SIZE, 0);
        assert(n >= 0);

        std::string message = "END";
        char send_data[64];
        memcpy(send_data, message.c_str(), message.size());

        n = zmq_send(broker, identify, strlen(identify), ZMQ_SNDMORE);
        assert(n > 0);
        n = zmq_send(broker, "", 0, ZMQ_SNDMORE);
        assert(n >= 0);
        n = zmq_send(broker, send_data, strlen(send_data), 0);
        assert(n > 0);
    }

    zmq_close(broker);
    zmq_term(context);
    return 0;
}