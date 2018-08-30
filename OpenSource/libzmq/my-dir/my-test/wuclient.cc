
// 发布-订阅
// 气象信息客户端
// 连接SUB套接字至tcp://*:5556端点
// 收集指定邮编的气象信息 并计算平均温度

#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    // 准备上下文 和订阅套接字
    void *context = zmq_init(1);
    void *subscriber = zmq_socket(context, ZMQ_SUB);

    // 连接远程地址
    int ret = zmq_connect(subscriber, "tcp://localhost:5556");
    assert(ret != -1);
    const char *filter = (argc > 1) ? argv[1] : "1001";
    // 设置套接字为订阅模式
    ret = zmq_setsockopt(subscriber, ZMQ_SUBSCRIBE, filter, strlen(filter));
    assert(ret != -1);

    printf("Collecting updates from weather server...\n");

    while (1) 
    {
        // 接收100条信息计算温度平均值
        long total_temp = 0;
        int update_nbr;
        for (update_nbr = 0; update_nbr < 100; ++update_nbr) 
        {
            char buf[256] = {'\0'};
            int n = zmq_recv (subscriber, buf, 255, 0);
            (void) n;
            int zipcode, temperature, relhumidity;
            sscanf (buf, "%d %d %d", &zipcode, &temperature, &relhumidity);
            total_temp += temperature;
        }

        printf ("Average temperature for zipcode '%s' was %dF\n", filter,
                (int)(total_temp / update_nbr));
        sleep(5);
    }
    zmq_close(subscriber);
    zmq_term(context);
    return 0;
}