
// 请求-应答消息流
// client获取请求并随机地分派给集群内的worker或其他集群

#include <zmq.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <vector>
#include <thread>
#include <memory>
#include <functional>
#include <queue>

#include <czmq.h>

#define BUFF_SIZE 128
#define NBR_CLIENTS 10
#define NBR_WORKERS 3
#define WORKER_READY "READY"

#define LOCAL_BACKEND_ADDR "ipc://%s-localbe.ipc"   // 提供给后端worker连接的地址
#define LOCAL_FRONTEND_ADDR "ipc://%s-localfe.ipc"  // 提供给前端客户端连接的地址

#define CLOUD_ADDR "ipc://%s-cloud.ipc" // 提供给别的集群连接的套接字地址 发起连接别的集群的套接字地址


// localfe
//  bind ZMQ_ROUTER  <-  REQ-task_thread

// localbe
//  bind ZMQ_ROUTER  <- REQ-worker_thread 

class client_task 
{
private:
    void *context_;
    void *client_;
public:
    client_task()
    {
        context_ = zmq_init(1);
        client_ = zmq_socket(context_, ZMQ_REQ);
    }
    // ~client_task()
    // {
    //     zmq_close(client_);
    //     zmq_term(context_);
    // }
    void start(const char *name)
    {
        char addr[BUFF_SIZE] = {'\0'};
        snprintf(addr, BUFF_SIZE, LOCAL_FRONTEND_ADDR, name);
        int ret = zmq_connect(client_, addr);
        assert(ret != -1);

        while (1)
        {
            // 发往集群的第2个请求 由client发往localfe-ROUTER
            // localfe-ROUTER收到请求后 会在LRU队列中拿出第一个worker的地址 将本请求转给该worker
            zstr_send(client_, "hello");
            char *reply = zstr_recv(client_);
            if (!reply) break;
            printf("client recv : [%s]\n", reply);
            free(reply);
            sleep(1);
        }
        zmq_close(client_);
        zmq_term(context_);
    }
};

class worker_task 
{
private:
    void *context_;
    void *worker_;
public:
    worker_task()
    {
        context_ = zmq_init(1);
        worker_ = zmq_socket(context_, ZMQ_REQ);
    }
    // ~worker_task()
    // {
    //     zmq_close(worker_);
    //     zmq_term(context_);
    // }
    void start(const char* name)
    {
        char addr[BUFF_SIZE] = {'\0'};
        snprintf(addr, BUFF_SIZE, LOCAL_BACKEND_ADDR, name);
        int ret = zmq_connect(worker_, addr);
        assert(ret != -1);        

        // 发往集群的第1个请求 worker通知localbe-ROUTER自己已经准备好
        // localbe-ROUTER收到后会将地址存入LRU队列
        zframe_t *frame = zframe_new(WORKER_READY, strlen(WORKER_READY));
        zframe_send(&frame, worker_, 0);

        while (1)
        {
            // worker收到集群localbe-ROUTER转发来的client请求
            zmsg_t *msg = zmsg_recv(worker_);
            if (!msg) break;
            
            zframe_print(zmsg_last(msg), "Worker : ");
            // 发往集群的第3个请求 worker对client的响应
            zframe_reset(zmsg_last(msg), "OK", 2);
            zmsg_send(&msg, worker_);
        }
        zmq_close(worker_);
        zmq_term(context_);
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
    char addr[BUFF_SIZE] = {'\0'};
    snprintf(addr, BUFF_SIZE, CLOUD_ADDR, self);
    printf("preparing broker at %s\n", addr);

    void *context = zmq_init(1);
    void *cloudfe = zmq_socket(context, ZMQ_ROUTER);
    zmq_setsockopt(cloudfe, ZMQ_IDENTITY, self, strlen(self));
    int ret = zmq_bind(cloudfe, addr);

    void *cloudbe = zmq_socket(context, ZMQ_ROUTER);
    zmq_setsockopt(cloudbe, ZMQ_IDENTITY, self, strlen(self));
    
    for (int argn = 2; argn < argc; ++argn)
    {
        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, CLOUD_ADDR, argv[argn]);
        printf("connecting to cloud forintend at '%s'\n", addr);
        ret = zmq_connect(cloudbe, addr);
        assert(ret != -1);
    }

    void *localfe = zmq_socket(context, ZMQ_ROUTER);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, LOCAL_FRONTEND_ADDR, self);    
    ret = zmq_bind(localfe, addr);
    assert(ret != -1);

    void *localbe = zmq_socket(context, ZMQ_ROUTER);
    memset(addr, 0, BUFF_SIZE);
    snprintf(addr, BUFF_SIZE, LOCAL_BACKEND_ADDR, self);
    ret = zmq_bind(localbe, addr);
    assert(ret != -1);

    printf("Press Enter when all brokers are started:");
    getchar();

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

    // LRU队列
    int capacity = 0;
    std::queue<zframe_t*> worker_queue;
    zmq_pollitem_t backends[] = 
    {
        { localbe, 0, ZMQ_POLLIN, 0 },
        { cloudbe, 0, ZMQ_POLLIN, 0 }
    };

    while (1)
    {
        ret = zmq_poll(backends, 2, capacity > 0 ? 1000 : -1);
        if (ret == -1) 
        {
            printf("zmq_poll ret -1\n");
            break;
        }
        zmsg_t *msg = NULL;
        // 优先处理后端worker的可读事件
        if (backends[0].revents & ZMQ_POLLIN)
        {
            // 收到后端worker发来的请求
            // READY-message or reply-message
            msg = zmsg_recv(localbe);
            if (!msg) break;

            zframe_t *address = zmsg_unwrap(msg);
            worker_queue.push(address);
            ++capacity; 

            zframe_t *frame = zmsg_first(msg);
            if (!memcmp(zframe_data(frame), WORKER_READY, strlen(WORKER_READY)))
            {
                zmsg_destroy(&msg);
            }
        }

        // 处理来自其他集群节点的应答消息
        if (backends[1].revents & ZMQ_POLLIN)
        {
            msg = zmsg_recv(cloudbe);
            if (!msg) break;
            // 暂不需要其他集群节点的地址
            zframe_t *address = zmsg_unwrap(msg);
            zframe_destroy(&address);
        }

        // 若之前将请求转给其他集群节点 且响应消息中的地址为其他集群节点的地址 则将消息发送给其他节点
        for (int argn = 2; msg && argn < argc; ++argn)
        {
            char *data = (char*)zframe_data(zmsg_first(msg));
            size_t size = zframe_size(zmsg_first(msg));
            if (size == strlen(argv[argn]) && memcmp(data, argv[argn], size) == 0)
            {
                zmsg_send(&msg, localfe);
            }
        }

        if (msg)
        {
            // 集群通过localfe-ROUTER将worker的响应转发给client
            zmsg_send(&msg, localfe);
        }

        // 开始处理 客户端client的请求 和 其他集群的请求
        while (capacity > 0)
        {
            zmq_pollitem_t frontends[] = 
            {
                { localfe, 0, ZMQ_POLLIN, 0 },
                { cloudfe, 0, ZMQ_POLLIN, 0 }
            };
            ret = zmq_poll(frontends, 2, 0);
            assert(ret != -1);
            
            int reroutable = 0;
            // 优先处理其他集群的请求 避免资源耗尽
            if (frontends[1].revents & ZMQ_POLLIN)
            {
                msg = zmsg_recv(cloudfe);
                reroutable = 0;
            }
            // 客户端请求
            else if (frontends[0].revents & ZMQ_POLLIN)
            {
                // 收到前端client发的的请求
                msg = zmsg_recv(localfe);
                reroutable = 1;
            }
            // 没有请求
            else 
            {
                break;
            }

            // 不是其他集群的请求 且存在其他集群 则将20%的请求发送给其他集群
            if (reroutable != 0 && argc > 2 && randof(5) == 0)
            {
                int idx = randof(argc - 1) + 1;
                zmsg_pushmem(msg, argv[idx], strlen(argv[idx]));
                // 将请求转给其他集群节点
                zmsg_send(&msg, cloudbe);
            }
            // 请求由自己处理
            else 
            {
                zframe_t *frame = worker_queue.front();
                worker_queue.pop();
                zmsg_wrap(msg, frame);
                // 集群将client的请求转给后端worker
                zmsg_send(&msg, localbe);
                --capacity;
            }
        }
    }

    // 资源释放
    while (!worker_queue.empty())
    {
        zframe_t *frame = worker_queue.front();
        worker_queue.pop();
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

    zmq_close(cloudfe);
    zmq_close(cloudbe);
    zmq_close(localfe);
    zmq_close(localbe);
    zmq_term(context);
    return 0;
}
