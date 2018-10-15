#include "freelancesvr.hpp"

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));
    FreeSvr *server = new FreeSvr();

    server->Bind("tcp://*:5555", "tcp://localhost:5555");
    server->Loop(verbose);

    delete server;
    return 0;
}
