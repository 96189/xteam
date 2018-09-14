
//

#include "bstarsrv.hpp"

int main(int argc, char const *argv[])
{
    assert(argc > 1);
    BStarSrv *bstarSrv = new BStarSrv();
    
    bstarSrv->Init(argv[1]);
    bstarSrv->Loop();

    delete bstarSrv;
    return 0;
}
