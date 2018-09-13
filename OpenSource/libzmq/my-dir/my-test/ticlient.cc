

//

#include "ticlient.hpp"

#define BROKER_ADDR "tcp://localhost:5555"

int main(int argc, char const *argv[])
{
    int verbose = (argc > 1 && streq(argv[1], "-v"));
    MdCli *session = new MdCli(BROKER_ADDR, verbose);

    session->Test();

    delete session;
    return 0;
}
