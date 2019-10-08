
// broker间状态流

#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 128

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("usage : ./binfile broker_name other_name1 ...");
        return 0;
    }
    // argv[1]为当前broker的名称
    char addr[BUFF_SIZE] = {'\0'};
    snprintf(addr, BUFF_SIZE, "ipc://%s-state.ipc", argv[1]);
    printf("preparing broker at %s\n", argv[1]);

    void *context = zmq_init(1);
    void *statebe = zmq_socket(context, ZMQ_PUB);
    int ret = zmq_bind(statebe, addr);
    assert(ret != -1);

    void *statefe = zmq_socket(context, ZMQ_SUB);
    zmq_setsockopt(statefe, ZMQ_SUBSCRIBE, "", 0);  // 订阅任何消息

    // argv[2] .. argv[n]是其他broker的名称
    for (int argn = 2; argn < argc; ++argn)
    {
        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, "ipc://%s-state.ipc", argv[argn]);
        printf("connecting to state backend at '%s'\n", argv[argn]);
        ret = zmq_connect(statefe, addr);   // 当前broker连接到其他broker 且订阅所有消息
        assert(ret != -1);
    } 

    int n = -1;
    char peername[BUFF_SIZE] = {'\0'};
    char available[BUFF_SIZE] = {'\0'};
    char buf[BUFF_SIZE] = {'\0'};

    zmq_pollitem_t items[] = 
    {
        { statefe, 0, ZMQ_POLLIN, 0 }
    };

    while (1)
    {
        memset(peername, 0, BUFF_SIZE);
        memset(available, 0, BUFF_SIZE);
        memset(buf, 0, BUFF_SIZE);
        ret = zmq_poll(items, 1, 1000);
        assert(ret != -1);
        
        if (items[0].revents & ZMQ_POLLIN)
        {
            // 处理收到的其他broker上报的 名称 和 空闲worker数
            n = zmq_recv(statefe, peername, BUFF_SIZE, 0);
            assert(n > 0);
            n = zmq_recv(statefe, available, BUFF_SIZE, 0);
            assert(n > 0);
            printf("%s - %s workers free\n", peername, available);
        } 
        else 
        {
            // 发送随机数标识空闲的worker数
            n = zmq_send(statebe, argv[1], strlen(argv[1]), ZMQ_SNDMORE);
            assert(n > 0);
            snprintf(buf, BUFF_SIZE, "%d", randof(10));
            n = zmq_send(statebe, buf, strlen(buf), 0);
        }
    }

    zmq_close(statebe);
    zmq_close(statefe);
    zmq_term(context);
    return 0;
}
