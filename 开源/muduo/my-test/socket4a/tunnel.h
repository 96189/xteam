#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/TcpServer.h>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

class Tunnel : public boost::enable_shared_from_this<Tunnel>, boost::noncopyable
{
public:
enum ServerClient
{
    kServer,
    kClient,
};
private:
    // Tcp tunnel作为客户端
    TcpClient client_;
    TcpConnectionPtr serverConn_;
    TcpConnectionPtr clientConn_;
public:
    // todo ???
    Tunnel(EventLoop* loop, const InetAddress& serverAddr, const TcpConnectionPtr& serverConn)
        : client_(loop, serverAddr, serverConn->name()),
          serverConn_(serverConn)
    {
        std::cout << "Tunnel " << serverConn->peerAddress().toIpPort() << " <-> " << serverAddr.toIpPort() << std::endl;        
    }
    ~Tunnel()
    {
        std::cout << "~Tunnel" << std::endl;
    }
    void connect()
    {
        client_.connect();
    }
    void disconnect()
    {
        client_.disconnect();
    }
    void setup()
    {
        client_.setConnectionCallback(boost::bind(&Tunnel::onClientConnection, shared_from_this(), _1));
        client_.setMessageCallback(boost::bind(&Tunnel::onClientMessage, shared_from_this(), _1, _2, _3));
        // todo ???
        serverConn_->setHighWaterMarkCallback(boost::bind(&Tunnel::onHighWaterMarkWeak, boost::weak_ptr<Tunnel>(shared_from_this()), kServer, _1, _2),
                                              1024 * 1024);
    }
    void teardown()
    {
        client_.setConnectionCallback(muduo::net::defaultConnectionCallback);
        client_.setMessageCallback(muduo::net::defaultMessageCallback);
        //
        if (serverConn_)
        {
            serverConn_->setContext(boost::any());
            serverConn_->shutdown();
        }
        clientConn_.reset();
    }
    // Tcp tunnel作为客户端
    // 连接建立
    void onClientConnection(const TcpConnectionPtr& conn)
    {
        std::cout << (conn->connected() ? "UP" : "DOWN") << std::endl;
        if (conn->connected())
        {
            conn->setTcpNoDelay(true);
            // todo ???
            conn->setHighWaterMarkCallback(boost::bind(&Tunnel::onHighWaterMarkWeak,boost::weak_ptr<Tunnel>(shared_from_this()), kClient, _1, _2),1024*1024);
            serverConn_->setContext(conn);
            serverConn_->startRead();
            clientConn_ = conn;
            if (serverConn_->inputBuffer()->readableBytes() > 0)
            {
                conn->send(serverConn_->inputBuffer());
            }
        }   
        else 
        {
            teardown();
        }
    }
    // Tcp tunnel作为客户端
    // 消息到达
    void onClientMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp time)
    {
        std::cout << conn->name() << " " << buf->readableBytes() << std::endl;
        if (serverConn_)
        {
            serverConn_->send(buf);
        }
        else 
        {
            buf->retrieveAll();
            abort();
        }
    }
    //
    //
    //
    void onHighWaterMark(ServerClient which,
                         const muduo::net::TcpConnectionPtr &conn,
                         size_t bytesToSent)
    {
        std::cout << (which == kServer ? "server" : "client")
                 << " onHighWaterMark " << conn->name()
                 << " bytes " << bytesToSent;

        if (which == kServer)
        {
            if (serverConn_->outputBuffer()->readableBytes() > 0)
            {
                clientConn_->stopRead();
                serverConn_->setWriteCompleteCallback(
                    boost::bind(&Tunnel::onWriteCompleteWeak,
                                boost::weak_ptr<Tunnel>(shared_from_this()), kServer, _1));
            }
        }
        else
        {
            if (clientConn_->outputBuffer()->readableBytes() > 0)
            {
                serverConn_->stopRead();
                clientConn_->setWriteCompleteCallback(
                    boost::bind(&Tunnel::onWriteCompleteWeak,
                                boost::weak_ptr<Tunnel>(shared_from_this()), kClient, _1));
            }
        }
    }

    static void onHighWaterMarkWeak(const boost::weak_ptr<Tunnel> &wkTunnel,
                                    ServerClient which,
                                    const muduo::net::TcpConnectionPtr &conn,
                                    size_t bytesToSent)
    {
        boost::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
        if (tunnel)
        {
            tunnel->onHighWaterMark(which, conn, bytesToSent);
        }
    }

    void onWriteComplete(ServerClient which, const muduo::net::TcpConnectionPtr &conn)
    {
        std::cout << (which == kServer ? "server" : "client")
                 << " onWriteComplete " << conn->name();
        if (which == kServer)
        {
            clientConn_->startRead();
            serverConn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
        }
        else
        {
            serverConn_->startRead();
            clientConn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
        }
    }

    static void onWriteCompleteWeak(const boost::weak_ptr<Tunnel> &wkTunnel,
                                    ServerClient which,
                                    const muduo::net::TcpConnectionPtr &conn)
    {
        boost::shared_ptr<Tunnel> tunnel = wkTunnel.lock();
        if (tunnel)
        {
            tunnel->onWriteComplete(which, conn);
        }
    }
};

typedef boost::shared_ptr<Tunnel> TunnelPtr;