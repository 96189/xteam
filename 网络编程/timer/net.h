#ifndef _NET_H_
#define _NET_H_

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#define SOCKET_INVALID          -1
#define SOCKET_CREATE_FAILED    -2
#define SOCKET_BIND_FAILED      -3
#define SOCKET_LISTEN_FAILED    -4

class CNET 
{
public:
    static int tcp_bind(const char* addr, uint16_t port, int backlog=5);
    static int tcp_accept(int netfd, struct sockaddr_in* addr);
    static int tcp_connect(int *netfd, const char* remote_addr, uint16_t port, int block=0);
};

#endif