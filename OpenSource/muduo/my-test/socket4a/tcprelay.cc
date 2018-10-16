#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <malloc.h>
#include <map>
#include "tunnel.h"

using namespace muduo;
using namespace muduo::net;

EventLoop* g_eventLoop;
InetAddress* g_serverAddr;
std::map<string, TunnelPtr> g_tunnels;

// 连接建立
void onServerConnection(const TcpConnectionPtr& conn)
{
    if (conn->connected())
    {
        conn->setTcpNoDelay(true);
        conn->stopRead();
        TunnelPtr tunnel(new Tunnel(g_eventLoop, *g_serverAddr, conn));
        // 设置回调函数
        tunnel->setup();
        // 发起连接
        tunnel->connect();
        g_tunnels[conn->name()] = tunnel;
    }
    else 
    {
        assert(g_tunnels.find(conn->name()) != g_tunnels.end());
        g_tunnels[conn->name()]->disconnect();
        g_tunnels.erase(conn->name());
    }
}

// 消息到达
void onServerMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
{
    if (!conn->getContext().empty())
    {
        const TcpConnectionPtr& clientConn = boost::any_cast<const TcpConnectionPtr&>(conn->getContext());
        clientConn->send(buf);
    }
}

void memstat()
{
    malloc_stats();
}

//
int main(int argc, char* argv[])
{
    if (argc >= 4)
    {
        // 限制内存
        {
            size_t kOneMB = 1024 * 1024;
            rlimit rl = { 256 * kOneMB, 256 * kOneMB };
            setrlimit(RLIMIT_AS, &rl);
        }
        // 设置服务端地址
        const char* ip = argv[1];
        uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
        InetAddress serverAddr(ip, port);
        g_serverAddr = &serverAddr;
        // 设置本机监听地址
        uint16_t acceptPort = static_cast<uint16_t>(atoi(argv[3]));
        InetAddress listenAddr(acceptPort);

        EventLoop loop;
        g_eventLoop = &loop;
        // todo ???
        // loop.runEvery(3, memstat);
        // TcpServer接受连接，客户端连接后，连接回调函数中创建tunnel，此时TcpServer作为客户端
        // 主动发起请求连接TcpServer的客户端感知不到TcpServer后端的tunnel
        TcpServer server(&loop, listenAddr, "TcpRelay");

        server.setConnectionCallback(onServerConnection);
        server.setMessageCallback(onServerMessage);
        server.start();
        loop.loop();
    }
    else 
    {
        fprintf(stderr, "Usage: %s <host_ip> <port> <listen_port>\n", argv[0]);
    }
    return 0;
}