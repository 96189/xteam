
#include "bstarreactor.hpp"

#define BSTAR_BACKUP 0
#define BSTAR_PRIMARY 1

// 具体的业务逻辑
int echo(zloop_t *loop, zmq_pollitem_t *poller, void *arg)
{
    zmsg_t *msg = zmsg_recv(poller->socket);
    zmsg_send(&msg, poller->socket);
    return 0;
}

int main(int argc, char const *argv[])
{
    BStarReactor *bstar = new BStarReactor();

    if (argc == 2 && streq(argv[1], "-p"))
    {
        printf("I: Primary active, waiting for backup (passive)\n");
        bstar->Init(BSTAR_PRIMARY, "tcp://*:5003", "tcp://localhost:5004"); // 初始化reactor loop
        bstar->Voter("tcp://*:5001", ZMQ_ROUTER, echo, NULL);   // 注册 业务逻辑
    }
    else if (argc == 2 && streq(argv[1], "-b"))
    {
        printf("I: Backup passive, waiting for primary (active)\n");
        bstar->Init(BSTAR_BACKUP, "tcp://*:5004", "tcp://localhost:5003");  // 初始化reactor loop
        bstar->Voter("tcp://*:5002", ZMQ_ROUTER, echo, NULL);   // 注册业务逻辑
    }
    else 
    {
        printf("Usage: bstarsrvs { -p | -b }\n");
        exit(0);
    }

    bstar->Start();     // 执行reactor loop

    delete bstar;
    return 0;
}
