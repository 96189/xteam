
// 分布式处理
// 有一组worker会处理这些任务
// 任务执行器
// 连接PULL套接字至tcp://localhost:5557端点
// 从任务分发器处获取任务
// 连接PUSH套接字至tcp://localhost:5558端点
// 向结果采集器发送结果
// 添加发布-订阅消息流 用于接收自杀消息
// 打开的套接字未显式关闭 会阻断进程的退出

#include <zmq.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

void task_exec(void *recevier, void *sender)
{
    char buf[65] = {'\0'};
    // 接收任务 任务内容是让收到的worker延迟若干毫秒
    int n = zmq_recv (recevier, buf, 64, 0);
    assert (n > 0);
    // 输出处理进度
    fflush (stdout);
    printf ("%s.", buf);
    fflush (stdout);

    // 开始处理
    usleep (atoi (buf) * 1000000 / 1000);

    // 发送处理结果
    n = zmq_send (sender, " ", 1, 0);
    assert (n > 0);
}

int main(int argc, char* argv[])
{
    // 准备上下文
    void *context = zmq_init(1);

    // 连接任务分发器 准备拉任务
    void *recevier = zmq_socket(context, ZMQ_PULL);
    int ret = zmq_connect(recevier, "tcp://localhost:5557");
    assert(ret != -1);

    // 连接结构收集器 准备推送执行结果
    void *sender = zmq_socket(context, ZMQ_PUSH);
    ret = zmq_connect(sender, "tcp://localhost:5558");
    assert(ret != -1);
    
    // worker接收sink发送的控制消息的套接字
    void *controller = zmq_socket(context, ZMQ_SUB);
    ret = zmq_connect(controller, "tcp://localhost:5559");
    assert(ret != -1);
    zmq_setsockopt(controller, ZMQ_SUBSCRIBE, "", 0);

    // 处理接收到的任务或控制消息
    zmq_pollitem_t items[] = 
    {
        { recevier, 0, ZMQ_POLLIN, 0 },
        { controller, 0, ZMQ_POLLIN, 0 }
    };

    while (1)
    {
        ret = zmq_poll(items, 2, -1);
        assert(ret != -1);

        // 处理任务
        if (items[0].revents & ZMQ_POLLIN)
        {
            task_exec(recevier, sender);
        }

        // 任何控制命令当前都按照结束进程生命
        if (items[1].revents & ZMQ_POLLIN)
        {
            break;
        }
    }
    zmq_close(recevier);
    zmq_close(sender);
    zmq_close(controller);
    zmq_term(context);
    return 0;
}