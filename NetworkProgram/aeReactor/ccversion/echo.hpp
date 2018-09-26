
//
#ifndef _ECHO_HPP_
#define _ECHO_HPP_

#include "aeEventLoop.hpp"
#include "aenet.hpp"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>


#define SERVER_PORT 9998
 
typedef struct
{
    char clientaddr[INET_ADDRSTRLEN];
    int port;
    char buf[1024];
}EchoBuf;

class Echo 
{

public:
    static void writeHandler(AEEventLoop *eventLoop, int fd, void *clientData, int mask)
    {
        int len = 0;
        char *buf = ((EchoBuf *)clientData)->buf;
        len = strlen(buf);

        printf("write to client: %s\n", buf);
        if (write(fd, buf, len) != len)
        {
            perror("write error");

            close(fd);
            eventLoop->aeDeleteFileEvent(fd, AE_READABLE);
            eventLoop->aeDeleteFileEvent(fd, AE_WRITABLE);

            printf("free %p\n", clientData);
            free(clientData);
        }
        eventLoop->aeDeleteFileEvent(fd, AE_WRITABLE);
    }
    static void readHandler(AEEventLoop *eventLoop, int fd, void *clientData, int mask)
    {
        char readbuf[1024] = {};
        int len = -1;
        EchoBuf *usrbuf = (EchoBuf *)clientData;

        if ((len = read(fd, readbuf, 1024)) > 0)
        {
            printf("read from <%s:%d>: %s\n", usrbuf->clientaddr, usrbuf->port, readbuf);

            memcpy(usrbuf->buf, readbuf, 1024);
            if (eventLoop->aeCreateFileEvent(fd, AE_WRITABLE, writeHandler, clientData) != AE_OK)
            {
                printf("aeCreateFileEvent error\n");
                goto END;
            }
            else
                return;
        }
        else if (len == 0)
        {
            printf("close link from %s\n", usrbuf->buf);
            goto END;
        }
        else
        {
            printf("read error from %s\n", usrbuf->buf);
            goto END;
        }

    END:
        close(fd);
        eventLoop->aeDeleteFileEvent(fd, AE_READABLE);
        eventLoop->aeDeleteFileEvent(fd, AE_WRITABLE);
        printf("free %p\n", clientData);
        free(clientData);
        return;
  }

    static void acceptTcpHandler(AEEventLoop *eventLoop, int fd, void *clientData, int mask)
    {
        int acceptfd = -1;
        struct sockaddr_in cliaddr;
        socklen_t addrlen = sizeof(cliaddr);

        acceptfd = accept(fd, (struct sockaddr *)&cliaddr, &addrlen);
        if (acceptfd < 0)
        {
            perror("accept error\n");
            return;
        }

        EchoBuf *usrbuf = (EchoBuf *)calloc(1, sizeof(EchoBuf));
        printf("calloc %p\n", usrbuf);
        inet_ntop(AF_INET, &cliaddr.sin_addr, usrbuf->clientaddr, INET_ADDRSTRLEN),
            usrbuf->port = ntohs(cliaddr.sin_port);
        printf("\naccept from <%s:%d>\n", usrbuf->clientaddr, usrbuf->port);

        // setunblock(acceptfd);
        anetNonBlock(NULL, acceptfd);

        if (eventLoop->aeCreateFileEvent(acceptfd, AE_READABLE, readHandler, usrbuf) != AE_OK)
        {
            perror("aeCreateFileEvent error");
            close(acceptfd);
            printf("free %p\n", usrbuf);
            free(usrbuf);
            return;
        }
        return;
    }
};

#endif