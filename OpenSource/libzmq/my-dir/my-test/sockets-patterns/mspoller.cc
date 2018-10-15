
// 处理多个套接字(仿照poll的实现 调用zmq_poll)

#include <zmq.h>
#include <assert.h>
#include <string.h>

void weather_update(void *subscriber, zmq_msg_t &message)
{
    printf("Collecting updates from weather server...\n");
    // 接收100条信息计算温度平均值
    long total_temp = 0;
    int update_nbr;
    char buf[256] = {'\0'};
    for (update_nbr = 0; update_nbr < 100; ++update_nbr) 
    {
        memset(buf, 0, 256);
        int n = zmq_msg_recv(&message, subscriber, 0);
        assert(n > 0);
        memcpy(buf, (char*)zmq_msg_data(&message), zmq_msg_size(&message));
        int zipcode, temperature, relhumidity;
        sscanf (buf, "%d %d %d", &zipcode, &temperature, &relhumidity);
        total_temp += temperature;
    }
    printf ("Average temperature for zipcode '%d' was %dF\n", 1001,
            (int) (total_temp / update_nbr));
}

void task_exec(void *recevier, zmq_msg_t &message)
{
    int n = zmq_msg_recv(&message, recevier, 0);
    assert(n > 0);
}

int main(int argc, char* argv[])
{
    void *context = zmq_init(1);

    // 连接任务分发器
    void *recevier = zmq_socket(context, ZMQ_PULL);
    int ret = zmq_connect(recevier, "tcp://localhost:5557");
    assert(ret != -1);

    // 连接气象更新服务
    void *subscriber = zmq_socket(context, ZMQ_SUB);
    ret = zmq_connect(subscriber, "tcp://localhost:5556");
    zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, "1001", 4);
    assert(ret != -1);
    
    // 初始化轮询对象 仿照poll的实现
    zmq_pollitem_t items[] = 
    {
        { recevier, 0, ZMQ_POLLIN, 0 },
        { subscriber, 0, ZMQ_POLLIN, 0 }
    };

    // 处理来自两个套接字的消息
    while (1)
    {
        zmq_msg_t message;
        // 仿照poll的实现 事件未发生则阻塞程序
        ret = zmq_poll(items, 2, -1);
        assert(ret != -1);
        if (items[0].revents & ZMQ_POLLIN)
        {
            zmq_msg_init(&message);
            // 处理任务
            task_exec(recevier, message);
            zmq_msg_close(&message);
        }
        if (items[1].revents & ZMQ_POLLIN)
        {
            zmq_msg_init(&message);
            // 处理气象更新
            weather_update(subscriber, message);
            zmq_msg_close(&message);
        }
    }
    zmq_close(recevier);
    zmq_close(subscriber);
    zmq_term(context);
    return 0;
}