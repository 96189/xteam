
// lazy pirate server
// bind REQ socket to tcp://*:5555
// like hwserver except:
//  - echoes request as-is
//  - randomly runs slowly or exits to simulate acrash

#include <zmq.h>
#include <czmq.h>
#include <assert.h>

#define BIND_ADDR "tcp://*:5555"

int main(int argc, char const *argv[])
{
    srandom((unsigned)time(NULL));
    void *context = zmq_init(1);
    void *server = zmq_socket(context, ZMQ_REP);

    int ret = zmq_bind(server, BIND_ADDR);
    assert(ret != -1);

    int cycles = 0;
    while (1)
    {
        char *request = zstr_recv(server);
        assert(request != NULL);
        ++cycles;

        if (cycles > 3 && randof(3) == 0)
        {
            printf("I: simulating a crash\n");
            break;
        }
        if (cycles > 3 && randof(3) == 0)
        {
            printf("I: simulating CPU overload\n");
            sleep(2);
        }
        printf("I: normal request (%s)\n", request);
        sleep(1);
        zstr_send(server, request);
        free(request);
    }
    zmq_close(server);
    zmq_term(context);
    return 0;
}
