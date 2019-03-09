
#include "adapter.h"

int main(int argc, char *argv[])
{
    Target *pTarget = new Adapter();
    pTarget->request();
    return 0;
}