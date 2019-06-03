#include "chargen.h"

#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>

#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

int main()
{
  LOG_INFO << "pid = " << getpid();
  // 外部提供的reactor实例 
  // 单线程情况下 本loop供acceptor和所有tcpconnection共享 
  // 配置线程池的情况下本loop仅仅供acceptor接收新连接 新连接tcpconnection共享整个线程池
  EventLoop loop;
  // 实例化地址
  InetAddress listenAddr(2019);
  ChargenServer server(&loop, listenAddr, true);
  // 开启tcpserver的线程池
  server.start();
  // 开启acceptor的事件循环
  loop.loop();
}

