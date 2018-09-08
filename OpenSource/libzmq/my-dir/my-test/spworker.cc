
// simple pirate worker
// connects REQ socket to tcp://*:5556
// implements worker part of load-balancing

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define BUFF_SIZE 128
#define WORKER_READY "READY"
#define BROKER_ADDR "tcp://localhost:5556"

int main(int argc, char const *argv[])
{
    srandom((unsigned)time(NULL));
    void *context = zmq_init(1);

    void *worker = zmq_socket(context, ZMQ_REQ);
    char identity[BUFF_SIZE] = {'\0'};
    snprintf(identity, BUFF_SIZE, "%04X-%04X", randof(0x10000), randof(0x10000));
    zmq_setsockopt(worker, ZMQ_IDENTITY, identity, strlen(identity));
    int ret = zmq_connect(worker, BROKER_ADDR);
    assert(ret != -1);

    printf ("I: (%s) worker ready\n", identity);
    zframe_t *frame = zframe_new(WORKER_READY, strlen(WORKER_READY));
    zframe_send(&frame, worker, 0);

    int cycles = 0;
    while (1)
    {
        zmsg_t *msg = zmsg_recv(worker);
        if (!msg) break;

        zframe_t *frame = zmsg_last(msg);
        printf("[%s] recv [%s]\n", identity, zframe_data(frame));
        ++cycles;
        if (cycles > 3 && randof(5) == 0)
        {
            printf ("I: (%s) simulating a crash\n", identity);
            zmsg_destroy(&msg);
            break;
        }

        if (cycles > 3 && randof(5) == 0)
        {
            printf ("I: (%s) simulating CPU overload\n", identity);
            sleep(3);
        }

        printf ("I: (%s) normal reply\n", identity);
        sleep(1);
        zmsg_send(&msg, worker);
    }

    zmq_close(worker);
    zmq_term(context);
    return 0;
}
