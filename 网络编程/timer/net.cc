#include "net.h"
#include "stdio.h"

int CNET::tcp_bind(const char* addr, uint16_t port, int backlog)
{
    int netfd = -1;
    int reuse_addr = 1;
    struct sockaddr_in inaddr;
	bzero(&inaddr, sizeof(struct sockaddr_in));
	inaddr.sin_family = AF_INET;
	inaddr.sin_port = htons(port);
    // 分离ip:port这类地址中的ip
	char *end = strchr((char *)addr, ':');
	if(end)
	{
		char *p = (char *)alloca(end-addr+1);
		memcpy(p, addr, end-addr);
		p[end-addr] = '\0';
		addr = p;
	}

    if(inet_pton(AF_INET, addr, &inaddr.sin_addr) <= 0)
	{
		printf("invalid address %s:%d", addr, port);
		return netfd;
	}
    
	if((netfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("make tcp socket error, %m");
		return -1;
	}    
    // 地址重用,禁止Nagle 算法
	setsockopt (netfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof (reuse_addr));
    setsockopt (netfd, SOL_TCP, TCP_NODELAY, &reuse_addr, sizeof (reuse_addr));

    if(::bind(netfd, (struct sockaddr *)&inaddr, sizeof(struct sockaddr)) == -1)
	{
		printf("bind tcp %s:%u failed, %m", addr, port);
		close (netfd);
		return -1;
	}

	if(::listen(netfd, backlog) == -1)
	{
		printf("listen tcp %s:%u failed, %m", addr, port);
		close (netfd);
		return -1;
	}
	printf("listen on tcp %s:%u\n", addr, port);
	return netfd;    
}

int CNET::tcp_connect(int *netfd, const char* remote_addr, uint16_t port, int block)
{
	*netfd = SOCKET_INVALID;    
	if ((*netfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)    
	{        
		return SOCKET_CREATE_FAILED;    
	}	

	sockaddr_in sock;	
    bzero((char*)&sock,sizeof(sock));	
	sock.sin_family = AF_INET;	
	sock.sin_port = htons(port);	

	sock.sin_addr.s_addr = inet_addr(remote_addr); 	
	socklen_t addrlen = sizeof(struct sockaddr_in);    
	int ret = ::connect (*netfd, (struct sockaddr *)&sock, addrlen);	
	if (!block)    
	{        
		fcntl (*netfd, F_SETFL, O_RDWR|O_NONBLOCK);    
	}    
	return ret;
}

int CNET::tcp_accept(int netfd, struct sockaddr_in* client_address)
{
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int newfd = ::accept(netfd, (struct sockaddr*)client_address, &addrlen);
    return newfd; 
}
