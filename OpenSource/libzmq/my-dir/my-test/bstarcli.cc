
//

#include "bstarcli.hpp"

int main(int argc, char const *argv[])
{
    BStarCli *bStarCli = new BStarCli("tcp://localhost:5001", "tcp://localhost:5002");

    bStarCli->Connect();
    bStarCli->Loop();

    delete bStarCli;
    return 0;
}
