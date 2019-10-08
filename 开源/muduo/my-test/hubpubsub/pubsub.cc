#include "pubsub.h"
#include "codec.h"
using namespace pubsub;
// 连接建立
void PubSubClient::onConnection(const TcpConnectionPtr &conn)
{
    if (conn->connected())
    {
        conn_ = conn;
    }
    else 
    {
        conn_.reset();
    }
    // 发布者注册发布回调函数
    // 订阅者注册订阅回调函数
    // 注册接口setConnectionCallback
    if (connectionCallback_)
    {
        connectionCallback_(this);
    }
}
// 消息到达
// 发布端不会有消息到达无需处理
// 这里只需要处理订阅端,订阅端处理pub命令,回调订阅处理函数
void PubSubClient::onMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp receiveTime)
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
            if (cmd == "pub" && subscribeCallback_)
            {
                subscribeCallback_(topic, content, receiveTime);
            }
        }
        else if (result == kError)
        {
            conn->shutdown();
        }
    }
}
bool PubSubClient::send(const string &message)
{
    bool succeed = false;
    if (conn_ && conn_->connected())
    {
        conn_->send(message);
        succeed = true;
    }
    return succeed;
}


// 订阅端调用
bool PubSubClient::subscribe(const string &topic, const SubscribeCallback &cb)
{
    string message = "sub " + topic + "\r\n";
    subscribeCallback_ = cb;
    return send(message);
}
void PubSubClient::unsubscribe(const string &topic)
{
    string message = "unsub " + topic + "\r\n";
    send(message);
}

// 发布端调用
bool PubSubClient::publish(const string &topic, const string &content)
{
    string message = "pub " + topic + "\r\n" + content + "\r\n";
    return send(message);
}