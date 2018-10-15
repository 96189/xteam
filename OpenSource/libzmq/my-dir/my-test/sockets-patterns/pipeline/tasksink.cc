
// 分布式处理
// 结果收集器会在末端接收所有worker的处理结果 进行汇总
// 任务收集器
// 绑定PULL套接字至tcp://localhost:5558端点
// 从worker处收集结果
// 添加发布-订阅消息流 用于向worker发送自杀信号
// 打开的套接字未显式关闭 会阻断进程的退出

#include <zmq.h>
#include <assert.h>
#include <sys/time.h>

#define TASK_NUMS 7

long long timeInMilliseconds(void) 
{
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

int main(int argc, char* argv[])
{
    // 准备上下文
    void *context = zmq_init(1);

    // 绑定本地套接字 准备拉消息
    void *recevier = zmq_socket(context, ZMQ_PULL);
    int ret = zmq_bind(recevier, "tcp://*:5558");
    assert(ret != -1);

    // 用于发送控制信息的套接字
    void *controller = zmq_socket(context, ZMQ_PUB);
    ret = zmq_bind(controller, "tcp://*:5559");
    assert(ret != -1);

    // 等待任务分发器发来的开始信号
    char buf[1];
    int n = zmq_recv(recevier, buf, 1, 0);
    assert(n > 0);

    // 开始计时
    long long start_time = timeInMilliseconds();

    // 确定TASK_NUMS个任务均已处理
    for (int task_nbr = 0; task_nbr < TASK_NUMS; ++task_nbr)
    {
        // 接收处理结果
        n = zmq_recv(recevier, buf, 1, 0);
        assert(n > 0);
        if ((task_nbr / 10) * 10 == task_nbr)
            printf(":");
        else 
            printf(".");
        fflush(stdout);                                   
    }
    // 计算并输出总执行时间
    printf("\nexecution time: %llu ms\n", (timeInMilliseconds() - start_time));
    
    zmq_send(controller, "KILL", 4, 0);

    zmq_close(recevier);
    zmq_close(controller);
    zmq_term(context);
    return 0;
}