
// 服务查询示例

#include "mdcliapi.hpp"

#define BROKER_ADDR "tcp://localhost:5555"

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));
    if (argc < 2)
    {
        printf("usage : ./bin -v service\n");
        return 0;
    }
    MdCli *session = new MdCli(BROKER_ADDR, verbose);
    session->Connect2Broker();
    
    const char *service = argv[2];
    zmsg_t *request = zmsg_new();
    zmsg_addstr(request, service);

    zmsg_t *reply = session->Send("mmi.service", &request);

    if (reply)
    {
        char *reply_code = zframe_strdup(zmsg_first(reply));
        printf("Lookup echo service: %s\n", reply_code);
        free(reply_code);
        zmsg_destroy(&reply);
    }
    else 
    {
        printf("E: no response from broker, make sure it's running\n");
    }
    delete session;
    return 0;
}
