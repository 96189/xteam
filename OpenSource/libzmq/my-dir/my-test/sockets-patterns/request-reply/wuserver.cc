
// 发布-订阅
// 气象信息更新服务
// 绑定PUB套接字至tcp://*:5556断点
// 发布随机气象信息

#include <zmq.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

int main(int argc, char* argv[])
{
    // 准备上下文和PUB套接字
    void *context = zmq_init(1);
    void *publisher = zmq_socket(context, ZMQ_PUB);
    
    // 套接字绑定某地址 及绑定频道
    int ret = zmq_bind(publisher, "tcp://*:5556");
    assert(ret != -1);
    ret = zmq_bind(publisher, "ipc://weather.ipc");
    assert(ret != -1);

    srandom((unsigned)time(NULL));
    while (1)
    {
        // 生成数据
        int zipcode, temperature, relhumidity;
        zipcode = randof(100000);
        temperature = randof(215) - 80;
        relhumidity = randof(50) + 10;

        // 向所有订阅者发布信息
        char update[256] = { '\0' };
        snprintf(update, 255, "%05d %d %d", zipcode, temperature, relhumidity);
        int n = zmq_send(publisher, update, strlen(update), 0);
        (void)n;
    }
    
    zmq_close(publisher);
    zmq_term(context);
    return 0;
}