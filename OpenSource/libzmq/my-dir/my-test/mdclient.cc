
// 

#include "mdcliapi.hpp"

#define BROKER_ADDR "tcp://localhost:5555"
#define LOOP_TIMES 100000

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));
    MdCli *session = new MdCli(BROKER_ADDR, verbose);
    int ret = session->Connect2Broker();
    assert(ret != -1);
    int count;

    char *service = "echo";
    for (count = 0; count < LOOP_TIMES; ++count)
    {
        zmsg_t *request = zmsg_new();
        zmsg_pushstr(request, "hello world");
        zmsg_t *reply = session->Send(service, &request);
        if (reply)
            zmsg_destroy(&reply);
        else 
            break;
    }
    printf("%d requests/replies processed\n", count);
    delete session;
    return 0;
}
