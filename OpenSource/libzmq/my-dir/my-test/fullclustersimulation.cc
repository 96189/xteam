
// 

#include <zmq.h>
#include <czmq.h>
#include <assert.h>
#include <stdlib.h>

#include <vector>
#include <thread>
#include <memory>
#include <functional>

#define BUFF_SIZE 128
#define NBR_CLIENTS 10
#define NBR_WORKERS 5
#define LRU_READY "READY"

#define LOCAL_BACKEND_ADDR "ipc://%s-localbe.ipc"   // 提供给后端worker连接的地址
#define LOCAL_FRONTEND_ADDR "ipc://%s-localfe.ipc"  // 提供给前端客户端连接的地址

#define STATE_ADDR "ipc://%s-state.ipc"             // 提供给别的集群节点连接的套接字地址 发起连接别的集群节点的套接字地址

#define CLOUD_ADDR "ipc://%s-cloud.ipc"             // 提供给别的集群节点连接的套接字地址 发起连接别的集群节点的套接字地址

#define MONITOR_ADDR "ipc://%s-monitor.ipc"         // 监控套接字地址

class client_task 
{
private:
    void *ctx_;
    void *client_;
    void *monitor_;

public:
    client_task()
    {
        ctx_ = zmq_init(1);
        client_ = zmq_socket(ctx_, ZMQ_REQ);
        monitor_ = zmq_socket(ctx_, ZMQ_PUSH);
    }
    void start(const char *name)
    {
        char addr[BUFF_SIZE] = {'\0'};
        snprintf(addr, BUFF_SIZE, LOCAL_FRONTEND_ADDR, name);
        int ret = zmq_connect(client_, addr);
        assert(ret != -1);        

        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, MONITOR_ADDR, name);
        ret = zmq_connect(monitor_, addr);

        while (1)
        {
            sleep(randof(5));
            int burst = randof(15);
            while (burst-- > 0)
            {
                char task_id[5];
                snprintf(task_id, 5, "%04X", randof(0x10000));

                // 发往集群的第2个请求 localfe-ROUTER client请求 
                zstr_send(client_, task_id);

                zmq_pollitem_t items[] = 
                {
                    { client_, 0, ZMQ_POLLIN, 0 }
                };

                ret = zmq_poll(items, 1, 10*1000);
                if (ret == -1) break;

                if (items[0].revents & ZMQ_POLLIN)
                {
                    // 接收响应
                    char *reply = zstr_recv(client_);
                    // char *addr = zstr_recv(client_);
                    // char *id = zstr_recv(client_);
                    
                    if (!reply/* || !id || !addr*/) break;
                    // printf("client recv reply : [%s %s -> %s]\n", addr, id, reply);
                    free(reply);
                    // free(addr);
                    // free(id);
                }
                // 10s超时未收到回复 认为任务丢失
                else 
                {
                    zstr_sendf(monitor_, "client quit, lose task is [%s]", task_id);
                    return;
                }
            }
        }

        zmq_close(client_);
        zmq_close(monitor_);
        zmq_term(ctx_);
    }
};

class worker_task 
{
private:
    void *ctx_;
    void *worker_;

public:
    worker_task()
    {
        ctx_ = zmq_init(1);
        worker_ = zmq_socket(ctx_, ZMQ_REQ);
    }
    void start(const char *name)
    {
        char addr[BUFF_SIZE] = {'\0'};
        snprintf(addr, BUFF_SIZE, LOCAL_BACKEND_ADDR, name);
        int ret = zmq_connect(worker_, addr);
        assert(ret != -1);

        // 发往集群的第1个请求 localbe-ROUTER 通知集群当前worker已经准备好
        zframe_t *frame = zframe_new(LRU_READY, strlen(LRU_READY));
        zframe_send(&frame, worker_, 0);

        while (1)
        {
            // 收到后端localbe-ROUTER转发来的请求消息
            zmsg_t *msg = zmsg_recv(worker_);
            sleep(randof(2));
            // 消息处理 为消息打标签 标记消息是集群中那个节点的哪个线程处理的
            // zmsg_addmem(msg, name, strlen(name));
            // memset(addr, 0, BUFF_SIZE);
            // snprintf(addr, BUFF_SIZE, "%ld", pthread_self());
            // zmsg_addmem(msg, addr, strlen(addr));
            // 处理消息将消息发还给localbe-ROUTER
            zmsg_send(&msg, worker_);
        }
        zmq_close(worker_);
        zmq_term(ctx_);
    }
};

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("usage : ./binfile broker_name other_name1 ...");
        return 0;
    }

    const char *self = argv[1];
    printf("preparing broker at %s...\n", self);
    srandom((unsigned)time(NULL));

    int ret = -1;
    char addr[BUFF_SIZE] = {'\0'};

    void *ctx = zmq_init(1);

    // localfe-ROUTER套接字绑定到地址
    void *localfe = zmq_socket(ctx, ZMQ_ROUTER);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, LOCAL_FRONTEND_ADDR, self);
    ret = zmq_bind(localfe, addr);
    assert(ret != -1);

    // localbe-ROUTER套接字绑定到地址
    void *localbe = zmq_socket(ctx, ZMQ_ROUTER);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, LOCAL_BACKEND_ADDR, self);
    ret = zmq_bind(localbe, addr);
    assert(ret != -1);

    // cloudfe-ROUTER套接字绑定到地址
    void *cloudfe = zmq_socket(ctx, ZMQ_ROUTER);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, CLOUD_ADDR, self);
    zmq_setsockopt(cloudfe, ZMQ_IDENTITY, self, strlen(self));
    ret = zmq_bind(cloudfe, addr);
    assert(ret != -1);

    // statebe-PUB套接字绑定到地址
    void *statebe = zmq_socket(ctx, ZMQ_PUB);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, STATE_ADDR, self);
    ret = zmq_bind(statebe, addr);
    assert(ret != -1);

    // 监控套接字绑定到地址
    void *monitor = zmq_socket(ctx, ZMQ_PULL);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, MONITOR_ADDR, self);
    ret = zmq_bind(monitor, addr);
    assert(ret != -1);

    // statefe-SUB套接字连接到其他集群节点
    void *statefe = zmq_socket(ctx, ZMQ_SUB);
    // 设置订阅消息
    zmq_setsockopt(statefe, ZMQ_SUBSCRIBE, "", 0);
    // cloudbe-ROUTER套接字连接其他集群节点
    void *cloudbe = zmq_socket(ctx, ZMQ_ROUTER);
    zmq_setsockopt(cloudbe, ZMQ_IDENTITY, self, strlen(self));

    for (int argn = 2; argn < argc; ++argn)
    {
        const char *peer = argv[argn];
        
        printf("connecting broker %s state frontend...\n", peer);
        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, STATE_ADDR, peer);
        ret = zmq_connect(statefe, addr);
        assert(ret != -1);

        printf("connecting broker %s cloud backend...\n", peer);
        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, CLOUD_ADDR, peer);
        ret = zmq_connect(cloudbe, addr);
        assert(ret != -1);
    }

    // start local workers
    std::vector<worker_task*> workers;
    std::vector<std::thread*> workers_thread;
    for (int worker_nbr = 0; worker_nbr < NBR_WORKERS; worker_nbr++)  
    {
        workers.push_back(new worker_task());
        workers_thread.push_back(new std::thread(std::bind(&worker_task::start, workers[worker_nbr], self)));
        workers_thread[worker_nbr]->detach();
    }

    // start local clients
    std::vector<client_task*> clients;
    std::vector<std::thread*> clients_thread;
    for (int client_nbr = 0; client_nbr < NBR_CLIENTS; client_nbr++)
    {
        clients.push_back(new client_task());
        clients_thread.push_back(new std::thread(std::bind(&client_task::start, clients[client_nbr], self)));
        clients_thread[client_nbr]->detach();
    }     


    int local_capacity = 0;
    int cloud_capacity = 0;

    zlist_t *lruqueue = zlist_new();

    while (1)
    {
        zmq_pollitem_t primary[] = 
        {
            { localbe, 0, ZMQ_POLLIN, 0 },
            { cloudbe, 0, ZMQ_POLLIN, 0 },
            { statefe, 0, ZMQ_POLLIN, 0 },
            { monitor, 0, ZMQ_POLLIN, 0 }
        };

        ret = zmq_poll(primary, sizeof(primary)/sizeof(primary[0]), local_capacity ? 1000 : -1);
        if (ret == -1) break;

        int previous = local_capacity;

        zmsg_t *msg = NULL;

        if (primary[0].revents & ZMQ_POLLIN)
        {
            // 集群localbe-ROUTER收到后端worker的请求
            // 可能是LRU_READY消息
            // 可能是对client请求的处理结果
            msg = zmsg_recv(localbe);
            if (!msg) break;

            // 去除请求中的地址入lru队列
            zframe_t *address = zmsg_unwrap(msg);
            assert(address != NULL);
            zlist_append(lruqueue, address);
            ++local_capacity;

            // 请求消息为LRU_READY则该消息不再传递
            zframe_t *frame = zmsg_first(msg);
            if (!memcmp(zframe_data(frame), LRU_READY, strlen(LRU_READY)))
            {
                zmsg_destroy(&msg);
            }
        }

        // 处理来自其他集群节点的应答消息
        if (primary[1].revents & ZMQ_POLLIN)
        {
            msg = zmsg_recv(cloudbe);
            if (!msg) break;
            zframe_t *address = zmsg_unwrap(msg);
            assert(address != NULL);
            printf("recv [%s] reply size %ld to node [%s]\n", (char*)zframe_data(address), zmsg_size(msg), (char*)zframe_data(zmsg_first(msg)));
            zframe_destroy(&address);
        }

        // 若之前将请求转给其他集群节点 且响应消息中的地址为其他集群节点的地址 则将消息发送给其他节点
        for (int argn = 2; msg && argn < argc; ++argn)
        {
            char *data = (char*)zframe_data(zmsg_first(msg));
            size_t size = zframe_size(zmsg_first(msg));
            if (size == strlen(argv[argn]) && !memcmp(data, argv[argn], size))
            {
                zmsg_send(&msg, cloudfe);
            }

        }

        // 集群通过localfe-ROUTER将worker的响应转发给client
        // 或者将其他集群的处理结果转发给client
        if (msg)
        {
            zmsg_send(&msg, localfe);
        }

        // 本集群节点statefe-SUB订阅其他集群节点statebe-PUB
        // 其他集群节点在statebe-PUB广播状态(可用worker数)变化信息
        if (primary[2].revents & ZMQ_POLLIN)
        {
            // 接收其他集群节点可用的worker数
            char *peer = zstr_recv(statefe);
            char *status = zstr_recv(statefe);
            cloud_capacity = atoi(status);
            // printf("recv notify cluster node [%s] has free %d worker\n", peer, cloud_capacity);
            free(peer);
            free(status);
        }

        // monitor套接字有可读事件
        // monitor套接字写的唯一位置是client发现发出去的请求在超时时间内没有得到响应
        if (primary[3].revents & ZMQ_POLLIN)
        {
            char *info = zstr_recv(monitor);
            printf("monitor : { %s }\n", info);
            free(info);
        }

        // 本地或者集群中有worker可用
        while (local_capacity + cloud_capacity)
        {
            zmq_pollitem_t secondary[] = 
            {
                { localfe, 0, ZMQ_POLLIN, 0 },
                { cloudfe, 0, ZMQ_POLLIN, 0 }
            };

            ret = zmq_poll(secondary, local_capacity ? 2 : 1, 0);
            assert(ret != -1);

            if (secondary[0].revents & ZMQ_POLLIN)
            {
                // 接收client发来的请求
                msg = zmsg_recv(localfe);
            }
            else if (secondary[1].revents & ZMQ_POLLIN)
            {
                // 接收其他集群节点转发来的请求
                msg = zmsg_recv(cloudfe);
                // printf("[%s] recv node [%s] trans req [%s]\n", 
                //        self,  (char*)zframe_data(zmsg_first(msg)), (char*)zframe_data(zmsg_last(msg)));
            }
            else 
            {
                break;
            }

            // 本地有worker可用 优先使用本地worker
            // randof(5) != 0 测试用 将20%的请求转给其他集群节点
            if (local_capacity && randof(5) != 0)
            {
                // 从lur队列中去除本地后端可用的worker地址
                zframe_t *frame = (zframe_t*)zlist_pop(lruqueue);
                assert(frame != NULL);
                zmsg_wrap(msg, frame);
                // 将请求消息转发给对应的worker
                zmsg_send(&msg, localbe);
                --local_capacity;
            }
            // 本地worker资源已耗尽 则使用其他节点的worker
            else 
            {
                int random_peer = randof(argc - 2) + 2;
                zmsg_pushmem(msg, argv[random_peer], strlen(argv[random_peer]));
                // 将请求消息转发给其他集群节点
                // printf("[%s] dispatch msg [%s] to [%s] node\n", 
                //        self, (char*)zframe_data(zmsg_last(msg)), argv[random_peer]);
                zmsg_send(&msg, cloudbe);
            }
        }
        // 当前集群节点中可用worker的数量发生了变化
        if (local_capacity != previous)
        {
            // 广播状态变化到整个集群中
            zstr_sendm(statebe, self);
            zstr_sendf(statebe, "%d", local_capacity);
            // printf("[%s] notify cluster nodes state [%s : %d]\n", self, self, local_capacity);
        }
    }


    // 资源释放
    while (zlist_size(lruqueue))
    {
        zframe_t *frame = (zframe_t*)zlist_pop(lruqueue);
        zframe_destroy(&frame);
    }

    for (int worker_nbr = 0; worker_nbr < NBR_WORKERS; worker_nbr++)
    {
        delete workers[worker_nbr];
        delete workers_thread[worker_nbr];
    }
    for (int client_nbr = 0; client_nbr < NBR_CLIENTS; client_nbr++)
    {
        delete clients[client_nbr];
        delete clients_thread[client_nbr];
    }

    zlist_destroy(&lruqueue);

    zmq_close(localfe);
    zmq_close(localbe);
    zmq_close(cloudfe);
    zmq_close(cloudbe);
    zmq_close(statebe);
    zmq_close(statefe);
    zmq_close(monitor);
    zmq_term(ctx);
    return 0;
}
