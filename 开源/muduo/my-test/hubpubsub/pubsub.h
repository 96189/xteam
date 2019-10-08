#include <muduo/net/TcpClient.h>
#include <boost/bind.hpp>

using namespace muduo;
using namespace muduo::net;

namespace pubsub
{
class PubSubClient : boost::noncopyable
{
public:
    typedef boost::function<void (PubSubClient*)> ConnectionCallback;
    typedef boost::function<void (const string& topic, const string& content, Timestamp)> SubscribeCallback;
private:
    // sub或者pub
    TcpClient client_;
    // sub或者pub与hub的连接
    TcpConnectionPtr conn_;
    ConnectionCallback connectionCallback_;
    SubscribeCallback subscribeCallback_;
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);
    bool send(const string& message);
public:
    PubSubClient(EventLoop* loop, const InetAddress& hubAddr, const string& name)
        : client_(loop, hubAddr, name)
    {
        client_.setConnectionCallback(boost::bind(&PubSubClient::onConnection, this, _1));
        client_.setMessageCallback(boost::bind(&PubSubClient::onMessage, this, _1, _2, _3));
    }
    void start()
    {
        client_.connect();
    }
    void stop()
    {
        client_.disconnect();
    }
    bool connected() const 
    {
        return conn_ && conn_->connected();
    }
    // 提供给外部调用 语义：订阅或发布
    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }
    bool subscribe(const string& topic, const SubscribeCallback& cb);
    void unsubscribe(const string& topic);
    bool publish(const string& topic, const string& content);
};
};