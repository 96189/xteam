
// Asynchronous client-to-server (DEALER to ROUTER)

#include <vector>
#include <thread>
#include <memory>
#include <functional>

#include <zmq.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

#define BUFF_SIZE 64

#define CLIENT_NUMS 3

// 生成一个位于 [0, num)区间的随机数
#define randof(num) (int)((float)(num) * random() / (RAND_MAX + 1.0))

#define CLIENT_CONNECT_ADDR "tcp://localhost:5570"

#define SERVER_FRONTEND_BIND_ADDR "tcp://*:5570"
#define SERVER_BACKEND_BIND_ADDR "inproc://backend"

void s_dump (void *socket);

// 客户端task类
// 连接到server 每秒发送1次请求 收集server响应并打印
// 客户端并行执行
// Clients connect to the server and send requests
// Clients can send multiple requests without waiting for a reply.
class client_task 
{
private:
    void *context_;
    void *client_socket_;

public:
    client_task()
    {
        context_ = zmq_init(1);
        client_socket_ = zmq_socket(context_, ZMQ_DEALER);
    }
    void start()
    {
        char identity [10] = {'\0'};
        sprintf (identity, "%04X-%04X", randof (0x10000), randof (0x10000));
        printf("client identity [ %s ]\n", identity);
        zmq_setsockopt(client_socket_, ZMQ_IDENTITY, identity, strlen(identity));   // 设置套接字标识
        int ret = zmq_connect(client_socket_, CLIENT_CONNECT_ADDR);
        assert(ret != -1);

        zmq_pollitem_t items[] = 
        {
            { client_socket_, 0, ZMQ_POLLIN, 0 }
        };

        int request_nbr = 0;
        while (1)
        {
            // 打印收到的回复(每秒取100次)
            // 100次*10ms 1秒取n个回复
            for (int i = 0; i < 100; ++i)
            {
                // 10 ms
                ret = zmq_poll(items, 1, 10);
                assert(ret != -1);
                if (items[0].revents & ZMQ_POLLIN)
                {
                    printf("\nreply client identity [ %s ]\n", identity);
                    s_dump(client_socket_);                                                 // 打印收到的回复
                }
            }

            // 每秒发送1次请求
            char request_string[16] = {'\0'};
            sprintf(request_string, "request #%d", ++request_nbr);
            int n = zmq_send(client_socket_, request_string, strlen(request_string), 0);    // 发送请求
            assert(n > 0);
        }
    }
};

// 服务端worker类
// For each request, the server sends 0 or more replies
// Servers can send multiple replies without waiting for new requests.
class server_worker 
{
private:
    void *context_;
    void *worker_;

public:
    server_worker(void *context, int sock_type)
        : context_(context)
    {
        worker_ = zmq_socket(context_, sock_type);
    }

    void work()
    {
        int ret = zmq_connect(worker_, SERVER_BACKEND_BIND_ADDR);
        assert(ret != -1);
        
        char identity[BUFF_SIZE] = {'\0'};
        char msg[BUFF_SIZE] = {'\0'};
        char copied_id[BUFF_SIZE] = {'\0'};
        char copied_msg[BUFF_SIZE] = {'\0'};
        while (1)
        {
            memset(identity, 0, BUFF_SIZE);
            memset(msg, 0, BUFF_SIZE);

            // 接收请求
            int n = zmq_recv(worker_, identity, BUFF_SIZE, 0);
            assert(n > 0);
            n = zmq_recv(worker_, msg, BUFF_SIZE, 0);
            assert(n > 0);

            // 随机发送N个回复
            int replies = randof(5);
            for (int reply = 0; reply < replies; ++reply)
            {
                memset(copied_id, 0, BUFF_SIZE);
                memset(copied_msg, 0, BUFF_SIZE);

                usleep((randof(1000) + 1) * 1000);
                // 构建回复内容
                memcpy(copied_id, identity, strlen(identity));
                memcpy(copied_msg, "reply ", 6);
                strcat(copied_msg, identity);
                strcat(copied_msg, msg);

                n = zmq_send(worker_, copied_id, strlen(copied_id), ZMQ_SNDMORE);
                assert(n > 0);
                n = zmq_send(worker_, copied_msg, strlen(copied_msg), 0);
                assert(n > 0);
            }
        }
    }
};

// 服务端task类(线程池)
class server_task 
{
private:
    void *context_;
    void *frontend_;
    void *backend_;

public:
    enum { kMaxThread = 5 };
    server_task()
    {
        context_ = zmq_init(1);
        frontend_ = zmq_socket(context_, ZMQ_ROUTER);
        backend_ = zmq_socket(context_, ZMQ_DEALER);
    }
    void run()
    {
        // 前后端套接字与地址绑定
        int ret = zmq_bind(frontend_, SERVER_FRONTEND_BIND_ADDR);
        assert(ret != -1);
        ret = zmq_bind(backend_, SERVER_BACKEND_BIND_ADDR);
        assert(ret != -1);

        // The server uses a pool of worker threads, each processing one request synchronously
        // 线程池和线程函数绑定
        std::vector<server_worker*> workers;
        std::vector<std::thread*> workers_thread;
        for (int i = 0; i < kMaxThread; ++i)
        {
            workers.push_back(new server_worker(context_, ZMQ_DEALER));
            workers_thread.push_back(new std::thread(std::bind(&server_worker::work, workers[i])));
            workers_thread[i]->detach();
        }

        // 内部队列 连接前后端套接字
        ret = zmq_proxy(frontend_, backend_, NULL);
        assert(ret != -1);

        // 线程池销毁
        for (int i = 0; i < kMaxThread; ++i)
        {
            delete workers[i];
            delete workers_thread[i];
        }
    }
};


int main(int argc, char const *argv[])
{
    // 使用多线程模拟 多进程架构
    // CLIENT_NUMS个客户端并行请求
    std::vector<client_task*> tasks;
    std::vector<std::thread*> tasks_thread;
    for (int i = 0; i < CLIENT_NUMS; ++i)
    {
        tasks.push_back(new client_task());
        tasks_thread.push_back(new std::thread(std::bind(&client_task::start, tasks[i])));
        tasks_thread[i]->detach();
    }

    // 1个服务端处理请求(服务端采用线程池模型)
    server_task st;
    std::thread t4(std::bind(&server_task::run, &st));

    getchar();

    return 0;
}

void s_dump(void *socket)
{
    int rc;

    zmq_msg_t message;
    rc = zmq_msg_init (&message);
    assert (rc == 0);

    puts ("----------------------------------------");
    //  Process all parts of the message
    do {
        int size = zmq_msg_recv (&message, socket, 0);
        assert (size >= 0);

        //  Dump the message as text or binary
        char *data = (char *) zmq_msg_data (&message);
        assert (data != 0);
        int is_text = 1;
        int char_nbr;
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            if ((unsigned char) data[char_nbr] < 32
                || (unsigned char) data[char_nbr] > 126) {
                is_text = 0;
            }
        }

        printf ("[%03d] ", size);
        for (char_nbr = 0; char_nbr < size; char_nbr++) {
            if (is_text) {
                printf ("%c", data[char_nbr]);
            } else {
                printf ("%02X", (unsigned char) data[char_nbr]);
            }
        }
        printf ("\n");
    } while (zmq_msg_more (&message));

    rc = zmq_msg_close (&message);
    assert (rc == 0);
}