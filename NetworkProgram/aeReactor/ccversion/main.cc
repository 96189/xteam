
#include "echo.hpp"

int main(int argc, char const *argv[])
{
    AEEventLoop *loop = new AEEventLoop(1024);

    int listenfd = anetTcpServer(NULL, SERVER_PORT, INADDR_ANY, 5);
 
    if (loop->aeCreateFileEvent(listenfd, AE_READABLE, Echo::acceptTcpHandler, NULL) != AE_OK)
    {
        perror("aeCreateFileEvent error");
        close(listenfd);
        delete loop;
        return -1;
    }
    loop->aeMain();

    // Echo *pEcho = new Echo();
    // pEcho->Init();
    // pEcho->Loop();
    return 0;
}
