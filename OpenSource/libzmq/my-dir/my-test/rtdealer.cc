
// 自定义ROUTER-DEALER路由
// ROUTER -> DEALER 是(1->n)的关系
// ROUTER根据每个DEALER设置的标识 来针对标识进行数据收发

#include <pthread.h>
#include <zmq.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

#define BUFF_SIZE 64

#define NBR_WORKERS 10

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

void *worker_task(void *args)
{
    void *context = zmq_init(1);

    void *worker = zmq_socket(context, ZMQ_DEALER);
    zmq_setsockopt(worker, ZMQ_IDENTITY, (char*)args, 1);   // worker为自己设置标识
    int ret = zmq_connect(worker, "ipc://routing.ipc");
    assert(ret != -1);

    int n = -1;
    int total = 0;
    char buf[BUFF_SIZE] = {'\0'};
    while (1)
    {
        memset(buf, 0, BUFF_SIZE);
        n = zmq_recv(worker, buf, BUFF_SIZE, 0);
        assert(n >= 0);

        if (!strncmp(buf, "END", 3))
        {
            printf("%s recevied : %d\n", (char*)args, total);
        }
        ++total;
    }

    zmq_close(worker);
    zmq_term(context);
    return NULL;
}

int main(int argc, char *argv[])
{
    void *context = zmq_init(1);

    void *broker = zmq_socket(context, ZMQ_ROUTER);
    int ret = zmq_bind(broker, "ipc://routing.ipc");
    assert(ret != -1);

    pthread_t worker;
    std::string argsA = "A";
    std::string argsB = "B";
    pthread_create(&worker, NULL, worker_task, const_cast<char*>(argsA.c_str()));
    pthread_create(&worker, NULL, worker_task, const_cast<char*>(argsB.c_str()));

    // ROUTER不知道DEALER何时准备好 简单的用sleep解决 实际应用中可采用信号处理
    // 采用sleep的方法可能导致随机的不稳定结果 一旦主线程先执行 则worker_task被执行到
    sleep(3);

    int n = -1;
    char buf[BUFF_SIZE] = {'\0'};
    strcpy(buf, "This is the workload");
    for (int task_nbr = 0; task_nbr < NBR_WORKERS; ++task_nbr)
    {
        // 路由采用随机加权进行负载均衡
        // >= 2/3的任务分配给A标识的套接字
        // 由ROUTER发送给DEALTER时需要知道标识
        if (randof(3) > 0)
        {
            n = zmq_send(broker, "A", 1, ZMQ_SNDMORE);
            assert(n > 0);
        }
        // < 1/3的任务分配给B标识的套接字
        // 由ROUTER发送给DEALTER时需要知道标识
        else 
        {
            n = zmq_send(broker, "B", 1, ZMQ_SNDMORE);
            assert(n > 0);
        }
        n = zmq_send(broker, buf, BUFF_SIZE, 0);
        assert(n > 0);
    }

    n = zmq_send(broker, "A", 1, ZMQ_SNDMORE);
    assert(n > 0);
    n = zmq_send(broker, "END", 3, 0);
    assert(n > 0);

    n = zmq_send(broker, "B", 1, ZMQ_SNDMORE);
    assert(n > 0);
    n = zmq_send(broker, "END", 3, 0);
    assert(n > 0);

    zmq_close(broker);
    zmq_term(context);
    return 0;
}