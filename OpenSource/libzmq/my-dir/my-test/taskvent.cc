
// 分布式处理
// 任务分发器会生成大量可以并行计算的任务
// 任务分发器
// 绑定PUSH套接字至tcp://localhost:5557端点
// 发送一组任务给已建立连接的worker
// 如何实现任务均衡分配?每个worker收到的任务基本均衡?如何保证任务不会重复分配且不漏掉?

#include <zmq.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))
#define TASK_NUMS 7

int main(int argc, char* argv[])
{
    // 准备上下文
    void *context = zmq_init(1);

    // 本地监听 主动PUSH消息的套接字
    void *sender = zmq_socket(context, ZMQ_PUSH);
    int ret = zmq_bind(sender, "tcp://*:5557");
    assert(ret != -1);

    // 连接远程结果收集器 准备PUSH消息
    void *sink = zmq_socket(context, ZMQ_PUSH);
    ret = zmq_connect(sink, "tcp://localhost:5558");
    assert(ret != -1);

    printf("Start the worker and press any key to start\n");
    getchar();
    printf("Assigning tasks to workers...\n");
    
    // 向连接成功的远程结果收集器 PUSH开始信号
    int n = zmq_send(sink, "0", 1, 0);
    assert(n > 0);

    srandom((unsigned)time(NULL));

    // 发送TASK_NUMS个任务
    int total_msec = 0;
    for (int task_nbr = 0; task_nbr < TASK_NUMS; ++task_nbr)
    {
        int workload;
        // 随机产生1到100ms的工作量
        workload = randof(100) + 1;
        total_msec += workload;
        char buf[65] = {'\0'};
        snprintf(buf, sizeof(int), "%d", workload);
        // 发送任务 任务内容是让收到的worker延迟若干毫秒
        n = zmq_send(sender, buf, strlen(buf), 0);
        assert(n > 0);
    }

    printf("Estimated execution time: %d ms\n", total_msec);

    sleep(1);

    zmq_close(sink);
    zmq_close(sender);
    zmq_close(context);
    return 0;
}