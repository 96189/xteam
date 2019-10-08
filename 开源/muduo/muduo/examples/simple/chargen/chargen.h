#ifndef MUDUO_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H
#define MUDUO_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H

#include <muduo/net/TcpServer.h>

// RFC 864
class ChargenServer
{
 public:
  ChargenServer(muduo::net::EventLoop* loop,
                const muduo::net::InetAddress& listenAddr,
                bool print = false);

  void start();

 private:
  // 连接建立回调
  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  // 消息到达回调
  void onMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buf, muduo::Timestamp time);
  // 写完成回调
  void onWriteComplete(const muduo::net::TcpConnectionPtr& conn);
  // 定时任务
  void printThroughput();

  muduo::net::TcpServer server_;

  muduo::string message_;
  int64_t transferred_;
  muduo::Timestamp startTime_;
};

#endif  // MUDUO_EXAMPLES_SIMPLE_CHARGEN_CHARGEN_H
