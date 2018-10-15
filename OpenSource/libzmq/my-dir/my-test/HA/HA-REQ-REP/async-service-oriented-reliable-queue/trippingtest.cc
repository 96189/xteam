
// Round-Trip模拟程序 同步异步性能对比

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define CLIENT_STR "hello, client"
#define WORKER_STR "hello, worker"
#define BROKER_STR "hello, broker"

#define CLIENT_CONNECT_ADDR "tcp://localhost:5555"
#define WORKER_CONNECT_ADDR "tcp://localhost:5556"
#define BROKER_FRONTEND_ADDR "tcp://*:5555"
#define BROKER_BACKEND_ADDR "tcp://*:5556"

#define LOOP_TIMES 100000

void client_task(zsock_t *pipe, void *args)
{
    assert(streq((char*)args, CLIENT_STR));
    zsock_signal(pipe, 0);

    zsock_t *client = zsock_new(ZMQ_DEALER);
    zsock_connect(client, CLIENT_CONNECT_ADDR);

    printf("Setting up test...\n");
    zclock_sleep(100);

    int requests;
    int64_t start;

    printf("Synchronous round-trip test...\n");
    start = zclock_time();
    for (requests = 0; requests < LOOP_TIMES; ++requests)
    {
        zstr_send(client, CLIENT_STR);
        zmsg_t *msg = zmsg_recv(client);
        zmsg_destroy(&msg);
    }
    printf(" %d calls/second\n", (1000 * LOOP_TIMES) / (int) (zclock_time () - start));

    printf ("Asynchronous round-trip test...\n");
    start = zclock_time();
    for (requests = 0; requests < LOOP_TIMES; ++requests)
    {
        zstr_send(client, CLIENT_STR);
    }
    for (requests = 0; requests < LOOP_TIMES; ++requests)
    {
        char *reply = zstr_recv(client);
        zstr_free(&reply);
    }
    printf(" %d calls/second\n", (1000 * LOOP_TIMES) / (int) (zclock_time () - start));

    zstr_send (pipe, "done");   // 信号通知
    zsock_destroy(&client);
}

void* worker_task(void *args)
{
    void *context = zmq_init(1);

    void *worker = zmq_socket(context, ZMQ_DEALER);
    zmq_connect(worker, WORKER_CONNECT_ADDR);

    while (1)
    {
        zmsg_t *msg = zmsg_recv(worker);
        zmsg_send(&msg, worker);
    }
    zmq_close(worker);
    zmq_term(context);
    return NULL;
}


void* broker_task(void *args)
{
    void *context = zmq_init(1);
    
    void *frontend = zmq_socket(context, ZMQ_DEALER);
    void *backend = zmq_socket(context, ZMQ_DEALER);
    zmq_bind(frontend, BROKER_FRONTEND_ADDR);
    zmq_bind(backend, BROKER_BACKEND_ADDR);
    zmq_proxy(frontend, backend, NULL);
    
    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return NULL;
}

int main(int argc, char const *argv[])
{
    zactor_t *client = zactor_new(client_task, (char*)CLIENT_STR);
    assert(client);

    pthread_t worker;
    pthread_create(&worker, NULL, worker_task, (char*)WORKER_STR);

    pthread_t broker;
    pthread_create(&broker, NULL, broker_task, (char*)BROKER_STR);

    char *signal = zstr_recv(client);
    printf("signal %s\n", signal);
    zstr_free(&signal);

    zactor_destroy(&client);
    printf("client done\n");

    return 0;
}
