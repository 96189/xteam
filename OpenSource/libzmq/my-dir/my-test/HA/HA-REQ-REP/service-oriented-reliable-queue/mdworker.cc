
//
#include "mdwrkapi.hpp"

#define BROKER_ADDR "tcp://localhost:5555"

int main(int argc, char const *argv[])
{
    char *service = "echo";
    int verbose = (argc > 1 && streq(argv[1], "-v"));
    MdWork *session = new MdWork(BROKER_ADDR, service, verbose);
    int ret = session->Connect2Broker();
    assert(ret != -1);
    zmsg_t *reply = NULL;
    while (1)
    {
        zmsg_t *request = session->Recv(&reply);
        if (!request) break;
        reply = request;
    }
    delete session;
    return 0;
}
