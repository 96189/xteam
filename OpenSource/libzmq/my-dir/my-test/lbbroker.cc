
// Load Balancing Broker 基于LRU算法
// clients(REQ) - [ ROUTER-frontend | broker | ROUTER-backend ] - workers(REQ)

// REQ套接字对发出的包会加上empty-delimiter frame
// ROUTER套接字对接收到的包会加上identity frame
// 由ROUTER-frontend收到的包由ROUTER-backend发出(在这里由我们自定义负载均衡算法)
// 会在broker进行重组为| client-identity | empty-delimiter | worker-identity | empty-delimiter | data |

#include <zmq.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <string>

#define NBR_CLIENTS 10
#define NBR_WORKERS 3
#define BUFF_SIZE 128

#define FRONTEND_ADDR "ipc://frontend.ipc"
#define BACKEND_ADDR "ipc://backend.ipc"

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

// basic request-reply client using REQ socket
void* client_thread(void *args)
{
    void *context = zmq_init(1);

    void *client = zmq_socket(context, ZMQ_REQ);
    char identity [10] = {'\0'};
    sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
    zmq_setsockopt (client, ZMQ_IDENTITY, identity, strlen (identity));     // 设置套接字标识符
    int ret = zmq_connect(client, FRONTEND_ADDR);                           // 连接broker的frontend端
    assert(ret != -1);

    // data frame
    int n = zmq_send(client, "hello", 5, 0);                                // 向broker的frontend端发起请求 
    assert(n > 0);
    printf("client %ld send request : %s/%s/%s\n", pthread_self(), identity, "", "hello");

    char reply[BUFF_SIZE] = {'\0'};
    n = zmq_recv(client, reply, BUFF_SIZE, 0);                              // 从broker的frontend端等待响应
    // printf("client %ld recv reply : %s\n", pthread_self(), reply);

    zmq_close(client);
    zmq_term(context);
    return NULL;
}

// worker using REQ socket to do LRU routing
void* worker_thread(void *args)
{
    void *context = zmq_init(1);

    void *worker = zmq_socket(context, ZMQ_REQ);
    char identity [10] = {'\0'};
    sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
    zmq_setsockopt (worker, ZMQ_IDENTITY, identity, strlen (identity));     // 设置套接字标识符
    int ret = zmq_connect(worker, BACKEND_ADDR);                            // 连接broker的backend端
    assert(ret != -1);
  
    int n = zmq_send(worker, "READY", 5, 0);                                // 通知broker的backend端当前后端已经准备好                 
    assert(n > 0);
    // printf("worker %ld send info : %s/%s/%s\n", pthread_self(), identity, "", "READY");

    char cliend_addr[BUFF_SIZE] = {'\0'};
    char delimiter[BUFF_SIZE] = {'\0'};
    char data[BUFF_SIZE] = {'\0'};
    while (1)
    {
        memset(cliend_addr, 0, BUFF_SIZE);
        memset(delimiter, 0, BUFF_SIZE);
        memset(data, 0, BUFF_SIZE);

        // client-identity frame
        // empty-delimiter frame
        // data frame
        n = zmq_recv(worker, cliend_addr, BUFF_SIZE, 0);                        // 从broker的backend端接收经过重组的请求
        assert(n >= 0);
        n = zmq_recv(worker, delimiter, BUFF_SIZE, 0);
        assert(n >= 0);
        n = zmq_recv(worker, data, BUFF_SIZE, 0);
        assert(n >= 0);
        // printf("worker %ld recv data : %s/%s/%s\n", pthread_self(), cliend_addr, "", data);

        n = zmq_send(worker, cliend_addr, strlen(cliend_addr), ZMQ_SNDMORE);    // 向broker的backend端返回对请求的处理结果
        assert(n > 0);
        n = zmq_send(worker, "", 0, ZMQ_SNDMORE);
        assert(n >= 0);
        n = zmq_send(worker, "OK", 2, 0);
        assert(n > 0);
        printf("worker %ld send reply : %s/%s/%s\n", pthread_self(), cliend_addr, "", "OK");
    }

    zmq_close(worker);
    zmq_term(context);
    return NULL;
}

int main(int argc, char *argv[])
{
    void *context = zmq_init(1);

    // 创建broker的frontend和backend的前后端套接字 类型都为ROUTER
    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    void *backend = zmq_socket(context, ZMQ_ROUTER);
    // 绑定broker的frontend套接字到某地址
    int ret = zmq_bind(frontend, FRONTEND_ADDR);
    assert(ret != -1);
    // 绑定broker的backend套接字到某地址
    ret = zmq_bind(backend, BACKEND_ADDR);
    assert(ret != -1);

    // 创建并执行NBR_CLIENTS个客户端
    int client_nbr = 0;
    for ( ; client_nbr < NBR_CLIENTS; ++client_nbr)
    {
        pthread_t client;
        pthread_create(&client, NULL, client_thread, (void*)&client_nbr);
    }

    // 创建并执行NBR_WORKERS个后端服务
    int worker_nbr = 0;
    for ( ; worker_nbr < NBR_WORKERS; ++worker_nbr)
    {
        pthread_t worker;
        pthread_create(&worker, NULL, worker_thread, (void*)&worker_nbr);
    }

    int n = -1;
    char client_addr[BUFF_SIZE] = {'\0'};
    char work_addr[BUFF_SIZE] = {'\0'};
    char delimiter[BUFF_SIZE] = {'\0'};
    char data[BUFF_SIZE] = {'\0'};
    std::queue<std::string> worker_queue;   // LRU队列

    while (1)
    {
        zmq_pollitem_t items[] = {
            { backend, 0, ZMQ_POLLIN, 0 },
            { frontend, 0, ZMQ_POLLIN, 0 }
        };
        ret = zmq_poll(items, worker_queue.size() ? 2 : 1, -1); // LRU队列中有数据 说明后端已连接 否则 后端尚未准备好
        assert(ret != -1);

        memset(client_addr, 0, BUFF_SIZE);
        memset(work_addr, 0, BUFF_SIZE);
        memset(delimiter, 0, BUFF_SIZE);
        memset(data, 0, BUFF_SIZE);

        // backend可读事件 worker_thread的写操作 READY或者client reply address
        if (items[0].revents & ZMQ_POLLIN)
        {
            // worker-identity (ROUTER add)
            n = zmq_recv(backend, work_addr, BUFF_SIZE, 0);
            assert(n >= 0);
            worker_queue.push(work_addr);
            // empty-delimiter (REQ add)
            n = zmq_recv(backend, delimiter, BUFF_SIZE, 0);
            assert(n >= 0);
            // frame (data or client-identity) (worker_thread add)
            n = zmq_recv(backend, client_addr, BUFF_SIZE, 0);
            assert(n >= 0);          
            // printf("broker backend recv pkg : %s/%s/%s\n", addr, delimiter, data);

            // 本次触发的写事件是由frontend转发过来的请求 或者
            // worker_thread的处理结果
            if (strncmp(client_addr, "READY", 5) != 0)
            {
                n = zmq_recv(backend, delimiter, BUFF_SIZE, 0);
                assert (n >= 0);
                n = zmq_recv(backend, data, BUFF_SIZE, 0);
                assert (n >= 0);


                n = zmq_send(frontend, client_addr, strlen(client_addr), ZMQ_SNDMORE);
                assert(n > 0);
                n = zmq_send(frontend, "", 0, ZMQ_SNDMORE);
                assert(n >= 0);
                n = zmq_send(frontend, data, strlen(data), 0);
                assert(n > 0);
                // printf("broker backend send pkg : %s/%s/%s\n", client_addr, "", data);

                if (--client_nbr == 0)
                    break;
            }
        }

        memset(client_addr, 0, BUFF_SIZE);
        memset(work_addr, 0, BUFF_SIZE);
        memset(delimiter, 0, BUFF_SIZE);
        memset(data, 0, BUFF_SIZE);

        // frontend可读事件
        if (items[1].revents & ZMQ_POLLIN)
        {
            // broker从frontend(ROUTER)接收client request
            // client-identity frame (ROUTER add)
            // empty-delimiter frame (REQ add)
            // data frame
            n = zmq_recv(frontend, client_addr, BUFF_SIZE, 0);
            assert(n >= 0);
            n = zmq_recv(frontend, delimiter, BUFF_SIZE, 0);
            assert(n >= 0);
            n = zmq_recv(frontend, data, BUFF_SIZE, 0);
            assert(n >= 0);
            // printf("broker frontend recv pkg : %s/%s/%s\n", client_addr, delimiter, data);

            // broker重新组包发给backend对应的thread_worker
            // worker-identity frame
            // empty-delimiter frame
            // client-identity frame
            // empty-delimiter frame
            // data frame
            std::string q_work_addr = worker_queue.front();
            worker_queue.pop();
            n = zmq_send(backend, q_work_addr.c_str(), q_work_addr.size(), ZMQ_SNDMORE);
            assert(n > 0);
            n = zmq_send(backend, "", 0, ZMQ_SNDMORE);
            assert(n >= 0);
            n = zmq_send(backend, client_addr, strlen(client_addr), ZMQ_SNDMORE);
            assert(n > 0);
            n = zmq_send(backend, "", 0, ZMQ_SNDMORE);
            assert(n >= 0);
            n = zmq_send(backend, data, strlen(data), 0);
            assert(n > 0);            
            // printf("broker frontend send pkg : %s/%s/%s/%s/%s\n", q_work_addr.c_str(), "", client_addr, "", data);
        }
    }

    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}