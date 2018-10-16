#include <stdio.h>
#include <string>
#include <set>
#include <map>
#include <boost/bind.hpp>
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Logging.h>
#include "codec.h"

using namespace muduo;
using namespace muduo::net;

namespace pubsub
{
//
class Topic : public muduo::copyable
{
  private:
    string topic_;
    string content_;
    Timestamp lastPubTime_;
    std::set<TcpConnectionPtr> audiences_;

  private:
    string makeMessage()
    {
        return "pub " + topic_ + "\r\n" + content_ + "\r\n";
    }

  public:
    Topic(const string &topic) : topic_(topic)
    {
    }
    // 订阅
    void add(const TcpConnectionPtr &conn)
    {
        audiences_.insert(conn);
        if (lastPubTime_.valid())
        {
            conn->send(makeMessage());
        }
    }
    // 取消订阅
    void remove(const TcpConnectionPtr &conn)
    {
        audiences_.erase(conn);
    }
    // 发布
    void publish(const string &content, Timestamp time)
    {
        content_ = content;
        lastPubTime_ = time;
        string message = makeMessage();
        for (std::set<TcpConnectionPtr>::iterator it = audiences_.begin(); it != audiences_.end(); ++it)
        {
            (*it)->send(message);
        }
    }
};

//
typedef std::set<string> ConnectionSubscription;
class PubSubServer : boost::noncopyable
{
  private:
    EventLoop *loop_;
    TcpServer server_;
    std::map<string, Topic> topics_;

  public:
    PubSubServer(EventLoop *loop, const muduo::net::InetAddress &listenAddr)
        : loop_(loop), server_(loop, listenAddr, "PubSubServer")
    {
        server_.setConnectionCallback(boost::bind(&PubSubServer::onConnection, this, _1));
        server_.setMessageCallback(boost::bind(&PubSubServer::onMessage, this, _1, _2, _3));
        // todo
        // loop_->runEvery(1.0, boost::bind(&PubSubServer::timePublish, this));
    }
    void start()
    {
        server_.start();
    }

  private:
    // 连接建立
    void onConnection(const TcpConnectionPtr &conn)
    {
        if (conn->connected())
        {
            //
            conn->setContext(ConnectionSubscription());
        }
        else
        {
            const ConnectionSubscription &connSub = boost::any_cast<const ConnectionSubscription &>(conn->getContext());
            for (ConnectionSubscription::const_iterator it = connSub.begin(); it != connSub.end();)
            {
                doUnsubscribe(conn, *it++);
            }
        }
    }
    // 消息到达
    void onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
    {
        ParseResult result = kSuccess;
        while (result == kSuccess)
        {
            string cmd;
            string topic;
            string content;
            result = parseMessage(buf, cmd, topic, content);
            if (result == kSuccess)
            {
                // 发布
                if (cmd == "pub")
                {
                    doPublish(conn->name(), topic, content, receiveTime);
                }
                // 订阅
                else if (cmd == "sub")
                {
                    doSubscribe(conn, topic);
                }
                // 取消订阅
                else if (cmd == "unsub")
                {
                    doUnsubscribe(conn, topic);
                }
                else
                {
                    result = kError;
                    conn->shutdown();
                }
            }
            else if (result == kError)
            {
                conn->shutdown();
            }
        }
    }
    // 订阅
    void doSubscribe(const TcpConnectionPtr &conn, const string &topic)
    {
        ConnectionSubscription *connSub = boost::any_cast<ConnectionSubscription>(conn->getMutableContext());
        connSub->insert(topic);
        // topic中保存着订阅该topic的tcpconnections
        getTopic(topic).add(conn);
    }
    // 取消订阅
    void doUnsubscribe(const TcpConnectionPtr &conn, const string &topic)
    {
        getTopic(topic).remove(conn);
        ConnectionSubscription *connSub = boost::any_cast<ConnectionSubscription>(conn->getMutableContext());
        connSub->erase(topic);
    }
    // 发布
    void doPublish(const string &source, const string &topic, const string &content, Timestamp time)
    {
        getTopic(topic).publish(content, time);
    }
    void timePublish()
    {
        Timestamp now = Timestamp::now();
        doPublish("internal", "utc_time", now.toFormattedString(), now);
    }

    Topic &getTopic(const string &topic)
    {
        std::map<string, Topic>::iterator it = topics_.find(topic);
        if (it == topics_.end())
        {
            it = topics_.insert(make_pair(topic, Topic(topic))).first;
        }
        return it->second;
    }
};

}; // namespace pubsub

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        uint16_t port = static_cast<uint16_t>(atoi(argv[1]));
        EventLoop loop;
        pubsub::PubSubServer server(&loop, InetAddress(port));
        server.start();
        loop.loop();
    }
    else
    {
        printf("Usage: %s pubsub_port [inspect_port]\n", argv[0]);
    }
    return 0;
}