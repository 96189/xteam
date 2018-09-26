#include "echo.hpp"

int main()
{
    aeEventLoop *eventloop = aeCreateEventLoop(1024);
    if (eventloop == NULL)
    {
        printf("aeCreateEventLoop error\n");
        return -1;
    }

    int listenfd = anetTcpServer(NULL, SERVER_PORT, INADDR_ANY, 5);
 
    if (aeCreateFileEvent(eventloop, listenfd, AE_READABLE, Echo::acceptTcpHandler, NULL) != AE_OK)
    {
        perror("aeCreateFileEvent error");
        close(listenfd);
        aeDeleteEventLoop(eventloop);
        return -1;
    }
 
    aeMain(eventloop);
    return 0;
}