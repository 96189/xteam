

#include "realSubject.h"
#include "proxySubject.h"

int main(int argc, char *argv[])
{
    std::string request = "GET HTTP /";
    IHttpInvokeSubject* pSubject = new ProxySubject(new RealSubject());
    pSubject->invoke(request);
    // delete pSubject;
    return 0;
}