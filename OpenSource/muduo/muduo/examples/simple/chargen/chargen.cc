#include "chargen.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <boost/bind.hpp>
#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

ChargenServer::ChargenServer(EventLoop* loop, const InetAddress& listenAddr, bool print)
  : server_(loop, listenAddr, "ChargenServer"),
    transferred_(0),
    startTime_(Timestamp::now())
{
  // TcpServer设置tcpconnection的连接建立回调
  server_.setConnectionCallback(boost::bind(&ChargenServer::onConnection, this, _1));
  // TcpServer设置tcpconnection的消息到达回调
  server_.setMessageCallback(boost::bind(&ChargenServer::onMessage, this, _1, _2, _3));
  // TcpServer设置tcpconnection的写完成回调
  server_.setWriteCompleteCallback(boost::bind(&ChargenServer::onWriteComplete, this, _1));
  
  if (print)
  {
    loop->runEvery(3.0, boost::bind(&ChargenServer::printThroughput, this));
  }

  string line;
  for (int i = 33; i < 127; ++i)
  {
    line.push_back(char(i));
  }
  line += line;

  for (size_t i = 0; i < 127-33; ++i)
  {
    message_ += line.substr(i, 72) + '\n';
  }
}

void ChargenServer::start()
{
  // TcpServer未配置线程池  默认acceptor和tcpconnection共享同一个loop(线程)
  // loop_->runInLoop(boost::bind(&Acceptor::listen, get_pointer(acceptor_)))
  // acceptor绑定listen函数并执行
  server_.start();
}

// 连接建立回调实现
void ChargenServer::onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << "ChargenServer - " << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " is " << (conn->connected() ? "UP" : "DOWN");
  if (conn->connected())
  {
    conn->setTcpNoDelay(true);
    conn->send(message_);
  }
}

// 消息到达回调实现
void ChargenServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
{
  string msg(buf->retrieveAllAsString());
  LOG_INFO << conn->name() << " discards " << msg.size() << " bytes received at " << time.toString();
}

// 写完成回调实现
void ChargenServer::onWriteComplete(const TcpConnectionPtr& conn)
{
  transferred_ += message_.size();
  conn->send(message_);
}

void ChargenServer::printThroughput()
{
  Timestamp endTime = Timestamp::now();
  double time = timeDifference(endTime, startTime_);
  printf("%4.3f MiB/s\n", static_cast<double>(transferred_)/time/1024/1024);
  transferred_ = 0;
  startTime_ = endTime;
}

