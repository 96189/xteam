#include "codec.h"

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <boost/bind.hpp>

#include <set>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class ChatServer : boost::noncopyable
{
 public:
  ChatServer(EventLoop* loop,
             const InetAddress& listenAddr)
  : server_(loop, listenAddr, "ChatServer"),
    codec_(boost::bind(&ChatServer::onStringMessage, this, _1, _2, _3))   /* 业务逻辑处理 */
  {
    // 连接建立回调
    server_.setConnectionCallback(
        boost::bind(&ChatServer::onConnection, this, _1));
    // 消息到达回调
    server_.setMessageCallback(
        boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
  }

  void start()
  {
    server_.start();
  }

 private:
  // 连接建立回调
  void onConnection(const TcpConnectionPtr& conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");

    if (conn->connected())
    {
      connections_.insert(conn);
    }
    else
    {
      connections_.erase(conn);
    }
  }

  // 业务层逻辑处理
  // 每收到1条消息 广播给所有连接
  void onStringMessage(const TcpConnectionPtr&,
                       const string& message,
                       Timestamp)
  {
    for (ConnectionList::iterator it = connections_.begin();
        it != connections_.end();
        ++it)
    {
      codec_.send(get_pointer(*it), message);
    }
  }

  typedef std::set<TcpConnectionPtr> ConnectionList;
  TcpServer server_;
  LengthHeaderCodec codec_;       // 编解码器(分包/打包) - 消息拦截器
  ConnectionList connections_;    // 连接表
};

int main(int argc, char* argv[])
{
  LOG_INFO << "pid = " << getpid();
  if (argc > 1)
  {
    EventLoop loop;
    uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
    InetAddress serverAddr(port);
    ChatServer server(&loop, serverAddr);
    server.start();
    loop.loop();
  }
  else
  {
    printf("Usage: %s port\n", argv[0]);
  }
}

