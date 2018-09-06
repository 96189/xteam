
// 

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

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

                zstr_send(client_, task_id);

                zmq_pollitem_t items[] = 
                {
                    { client_, 0, ZMQ_POLLIN, 0 }
                };

                ret = zmq_poll(items, 1, 10*1000);
                if (ret == -1) break;

                if (items[0].revents & ZMQ_POLLIN)
                {
                    char *reply = zstr_recv(client_);
                    if (!reply) break;
                    puts(reply);
                    assert(streq(reply, task_id));
                    free(reply);
                }
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

        zframe_t *frame = zframe_new(LRU_READY, strlen(LRU_READY));
        zframe_send(&frame, worker_, 0);

        while (1)
        {
            zmsg_t *msg = zmsg_recv(worker_);
            sleep(randof(2));
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
    // cloudbe-ROUTER套接字连接其他集群节点
    void *cloudbe = zmq_socket(ctx, ZMQ_ROUTER);
    zmq_setsockopt(cloudbe, ZMQ_IDENTITY, self, strlen(self));

    for (int argn = 2; argn < argc; ++argn)
    {
        const char *peer = argv[argn];
        
        printf("connecting broker %s state frontend...\n", peer);
        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, STATE_ADDR, self);
        ret = zmq_connect(statefe, addr);
        assert(ret != -1);

        printf("connecting broker %s cloud backend...\n", peer);
        memset(addr, 0, BUFF_SIZE);
        snprintf(addr, BUFF_SIZE, CLOUD_ADDR, self);
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


    int local_capacity;
    int cloud_capacity;

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
            msg = zmsg_recv(localbe);
            if (!msg) break;
            zframe_t *address = zmsg_unwrap(msg);
            zlist_append(lruqueue, address);
            ++local_capacity;

            zframe_t *frame = zmsg_first(msg);
            if (!memcmp(zframe_data(frame), LRU_READY, strlen(LRU_READY)))
            {
                zmsg_destroy(&msg);
            }
        }

        if (primary[1].revents & ZMQ_POLLIN)
        {
            msg = zmsg_recv(cloudbe);
            if (!msg) break;
            zframe_t *address = zmsg_unwrap(msg);
            zframe_destroy(&address);
        }

        for (int argn = 2; argn < argc; ++argn)
        {
            char *data = (char*)zframe_data(zmsg_first(msg));
            size_t size = zframe_size(zmsg_first(msg));
            if (size == strlen(argv[argn]) && !memcmp(data, argv[argn], size))
            {
                zmsg_send(&msg, cloudfe);
            }
        }

        if (msg)
        {
            zmsg_send(&msg, localfe);
        }

        if (primary[2].revents & ZMQ_POLLIN)
        {
            char *status = zstr_recv(statefe);
            cloud_capacity = atoi(status);
            free(status);
        }

        if (primary[3].revents & ZMQ_POLLIN)
        {
            char *info = zstr_recv(monitor);
            printf("monitor : [%s]\n", info);
            free(info);
        }

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
                msg = zmsg_recv(localfe);
            }
            else if (secondary[1].revents & ZMQ_POLLIN)
            {
                msg = zmsg_recv(cloudfe);
            }
            else 
            {
                break;
            }

            if (local_capacity)
            {
                zframe_t *frame = (zframe_t*)zlist_pop(lruqueue);
                zmsg_wrap(msg, frame);
                zmsg_send(&msg, localbe);
                --local_capacity;
            }
            else 
            {
                int random_peer = randof(argc - 2) + 2;
                zmsg_pushmem(msg, argv[random_peer], strlen(argv[random_peer]));
                zmsg_send(&msg, cloudbe);
            }
        }
        if (local_capacity != previous)
        {
            zstr_sendm(statebe, self);
            zstr_sendf(statebe, "%d", local_capacity);
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
