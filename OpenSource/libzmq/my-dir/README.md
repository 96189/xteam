## 一、消息模式
### 0x01 Request-Reply
    Socket-type:
        ZMQ_REQ ZMQ_REP
![request-reply](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/request-reply.png)

    实际应用:mtserver.cc hwclient.cc
![multithreaded-server](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/multithreaded-server.png)

### 0x02 Publish-Subscribe
    Socket-type:
        ZMQ_PUB ZMQ_SUB
![publish-subscribe](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/publish-subscribe.png)

    实际应用:wuserver.cc wuproxy.cc wuclient.cc
![pub-sub-forwarder-proxy](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/pub-sub-forwarder-proxy.png)

### 0x03 Parallel Pipeline
    Socket-type:
        ZMQ_PUSH ZMQ_PULL
![parallel-pipeline](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/parallel-pipeline.png)
    
    实际应用:taskvent.cc taskwork.cc tasksink.cc
![prarllel-pipeline-with-kill-signal](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/prarllel-pipeline-with-kill-signal.png)

#### 1、负载均衡(上半部)

#### 2、公平队列(下半部)
![fair-queuing](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/fair-queuing.png)

### 0x04 Exclusive pair


## 二、合法的套接字连接-绑定对
### 0X01 REQ-REP
#### 1、Request Distribution   
![request-distribution](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/request-distribution.png)
    
    n(clients) : n(services)
    n个客户端连接到REQ套接字,由REQ套接字做负载均衡,将请求均匀的分摊到后端services.
    优:低成本的添加 客户端和服务
    缺:客户端需要需要知道service拓扑,若新增后端service则需要客户端重启,以便新的service可以生效

#### 2、Request-Reply Broker
![request-reply-broker](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/request-reply-broker.png)
    
    n(clients) : n(services)

### 0x02 PUB-SUB
#### 1、Small-Scale Pub-Sub Network
![small-scale-pub-sub-network](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/small-scale-pub-sub-network.png)
    
    1(publisher) -> n(subscriber)

#### 2、Pub-Sub Network with a Proxy
![pub-sub-network-with-a-proxy](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/pub-sub-network-with-a-proxy.png)
    
    n(publisher) -> n(subscriber)

### 0x03 PUSH-PULL

### 0x04 REQ-ROUTER
### 0x05 DEALER-ROUTER

### 0x06 DEALER-DEALER
### 0x07 ROUTER-ROUTER

### 0x08 PAIR-PAIR

## 代理(proxy or broker)
### 代理解决的问题
    "dynamic discovery problem"
        最简单的是 静态发现 手工完成(hard-coding or configuring) 
        动态发现 pub-sub代理解决动态发现问题
    伸缩性(随时添加和删除前后端节点)
        ROUTER(负载均衡)-DEALER(公平队列)代理
    隔离(桥接)
        内外网隔离: 通过SUB-PROXY-PUB代理
### ZMQ提供的代理
    QUEUE       REP-REQ代理
    FORWARDER   PUB-SUB代理
    STREAMER    pipeline代理

图片来源:http://zguide.zeromq.org/page:all