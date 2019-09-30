
#include "adapter.h"

int main(int argc, char *argv[])
{
    Target *pTarget = new Adapter(new Adaptee());
    pTarget->request();
    return 0;
}