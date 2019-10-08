
// simple pirate broker
// this is identical to load-balancing pattern, with no reliability mechanisms.
// it depends on the client for recovery. runs forever.

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define BUFF_SIZE 128
#define WORKER_READY "READY"
#define FRONTEND_ADDR "tcp://*:5555"
#define BACKEND_ADDR "tcp://*:5556"

int main(int argc, char const *argv[])
{
    void *context = zmq_init(1);

    void *frontend = zmq_socket(context, ZMQ_ROUTER);
    void *backend = zmq_socket(context, ZMQ_ROUTER);
    int ret = zmq_bind(frontend, FRONTEND_ADDR);
    assert(ret != -1);
    ret = zmq_bind(backend, BACKEND_ADDR);
    assert(ret != -1);

    zlist_t *lruqueue = zlist_new();

    while (1)
    {
        zmq_pollitem_t items[] = 
        {
            { backend, 0, ZMQ_POLLIN, 0 },
            { frontend, 0, ZMQ_POLLIN, 0 }
        };

        ret = zmq_poll(items, zlist_size(lruqueue) ? 2 : 1, -1);
        if (ret == -1) break;

        if (items[0].revents & ZMQ_POLLIN)
        {
            zmsg_t *msg = zmsg_recv(backend);
            if (!msg) break;

            zframe_t *address = zmsg_unwrap(msg);
            zlist_append(lruqueue, address);

            zframe_t *frame = zmsg_first(msg);
            // worker已准备通知
            if (!memcmp(WORKER_READY, zframe_data(frame), strlen(WORKER_READY)))
            {
                zmsg_destroy(&msg);
            }
            // worker对请求的响应转给frontend
            else 
            {   
                zmsg_send(&msg, frontend);
            }
        }

        if (items[1].revents & ZMQ_POLLIN)
        {
            // frontend接收到请求 在队列中选出worker 将请求发给worker
            zmsg_t *msg = zmsg_recv(frontend);
            if (msg)
            {
                zframe_t* worker_addr = (zframe_t*)zlist_pop(lruqueue);
                assert(worker_addr != NULL);
                zmsg_wrap(msg, worker_addr);
                zmsg_send(&msg, backend);
            }
        }
    }

    while (zlist_size(lruqueue))
    {
        zframe_t *frame = (zframe_t*)zlist_pop(lruqueue);
        assert(frame != NULL);
        zframe_destroy(&frame);
    }

    zmq_close(frontend);
    zmq_close(backend);
    zmq_term(context);
    return 0;
}

