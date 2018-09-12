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

### 代理(proxy or broker)
#### 代理解决的问题
    "dynamic discovery problem"
        最简单的是 静态发现 手工完成(hard-coding or configuring) 
        动态发现 pub-sub代理解决动态发现问题
    伸缩性(随时添加和删除前后端节点)
        ROUTER-DEALER代理
    隔离(桥接)
        内外网隔离: 通过SUB-PROXY-PUB代理
#### ZMQ提供的代理
    QUEUE       REP-REQ代理
    FORWARDER   PUB-SUB代理
    STREAMER    pipeline代理

### 自定义请求-响应路由
#### ROUTER-REQ模式
    rtreq.cc
    1(ROUTER) -> n(REQ)
    1->n模式下 ROUTER如何分辨n个DEALER?

    Routing Envelope for REQ
        | address | empty | data |
![routing-envelope-for-req](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/routing-envelope-for-req.png)

    应用实例:
        mtserver.cc 同时与多个REQ客户端通信的异步服务器

#### ROUTER-DEALER模式
    rtdealer.cc
    1(ROUTER) -> n(DEALER)
    1->n模式下 ROUTER如何分辨n个DEALER?(消息分发)

    Routing Envelope for DEALER
        | address | data | 数据传输前手工构造信封,使用已知的标识发送数据,若定义非法的信封地址ROUTER会丢弃该消息,不做任何提示

    为异步客户端提供了与异步服务器通信的能力,双方可以完全控制消息格式.DEALER和ROUTER都可以使用任意消息格式,如果您希望安全地使用它们,您必须成为一个协议设计者.至少你必须决定是否要模仿REQ / REP回复信封.这取决于你是否真的需要发送回复.

#### REQ和DEALER的区别
    Anywhere you can use REQ, you can use DEALER. There are two specific differences:
        The REQ socket always sends an empty delimiter frame before any data frames; the DEALER does not.
        The REQ socket will send only one message before it receives a reply; the DEALER is fully asynchronous.

### 套接字模式的无效组合
        Mostly, trying to connect clients to clients, or servers to servers is a bad idea and won't work. However, 
    rather than give general vague warnings, I'll explain in detail:

        REQ to REQ: both sides want to start by sending messages to each other, and this could only work if you timed things so that both peers exchanged messages at the same time. It hurts my brain to even think about it.
        
        REQ to DEALER: you could in theory do this, but it would break if you added a second REQ because DEALER has no way of sending a reply to the original peer. Thus the REQ socket would get confused, and/or return messages meant for another client.
        
        REP to REP: both sides would wait for the other to send the first message.
        REP to ROUTER: the ROUTER socket can in theory initiate the dialog and send a properly-formatted request, if it knows the REP socket has connected and it knows the identity of that connection. It's messy and adds nothing over DEALER to ROUTER.

        The common thread in this valid versus invalid breakdown is that a ZeroMQ socket connection is always biased 
    towards one peer that binds to an endpoint, and another that connects to that. Further, that which side binds and 
    which side connects is not arbitrary, but follows natural patterns. The side which we expect to "be there" binds: 
    it'll be a server, a broker, a publisher, a collector. The side that "comes and goes" connects: it'll be clients and 
    workers. Remembering this will help you design better ZeroMQ architectures.

### clients(REQ) - [ROUTER-frontend | broker | ROUTER-backend] - workers(REQ)
![load-balancing-broker](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/load-balancing-broker.png)

    lbbroker.cc
    由REQ发出的包会自动加上empty-delimiter frame
    由ROUTER收到的包会自动加上identity frame
    由ROUTER发出的包会自动去掉信封(identity frame和empty-delimiter frame)

    client发出的包(data frame) 
    首先会加上empty-deimiter frame 
    在ROUTER-frontend会加上client-identity frame
    在ROUTER-backend会加上empty-delimiter frame和worker-identity frame
    此时包结构为 | worker-identity | empty-delimiter | client-identity | empty-deimiter | data |

    由ROUTER-backend发出去掉信封worker收到的包结构为
    | client-identity | empty-deimiter | data |

    lbbroker.cc在ROUTER-backend基于LRU算法实现了负载均衡,事实上ROUTER-backend可以替换为ZMQ_DEALER且用ZMQ_QUEUE来连接前后端,这样的例子已在mtserver.cc中实现.ZMQ_DEALER自带负载均衡.

### clients(DEALER) - server(ROUTER)
    asyncsrv.cc
    异步套接字模式 DEALER

![detail-of-asynchronous-server](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/detail-of-asynchronous-server.png)

        clients与server(n:1)之间采用DEALER to ROUTER的模式

        server采用线程池模型 server内部main thread与worker thread(1:n)采用DEALER to DEALER的模式,DEALER是异步模式套接字,若有
    同步需求则需要REP套接字.

    数据包
         client          server       frontend       worker
    [ DEALER ]<---->[ ROUTER <----> DEALER <----> DEALER ]
            data-frame                  data-frame identity-frame
                      data-frame identity-frame       

    To properly manage client state in a stateful asynchronous server, you have to:
        Do heartbeating from client to server. In our example, we send a request once per second, which can reliably be 
    used as a heartbeat.
        Store state using the client identity (whether generated or explicit) as key.
        Detect a stopped heartbeat. If there's no request from a client within, say, two seconds, the server can detect 
    this and destroy any state it's holding for that client.

### Inter-Broker Routing(跨代理路由)
#### 需求
        design of a large cloud computing facility. He has this vision of a cloud that spans many data centers, each a 
    cluster of clients and workers, and that works together as a whole. 

#### 需求分析
* worker在不同的硬件上运作 但可以处理所有类型的任务 每个集群都有成百个worker 再乘以集群的个数 因此数量众多
* client向worker指派任务 每个任务都是独立的 每个client都希望能找到对应的worker来处理任务 越快越好 client是不固定的 来去频繁
* 真正的难点在于 这个架构需要能够自如地添加和删除集群 附带着集群中的client和worker
* 如果集群中没有可用的worker 它便会将任务分派给其他集群中可以用的worker
* client每次发送一个请求 并等待应答 如果X秒后他们没有获得应答 他们会重新发送请求 这一点我们不需要多做考虑 client端的API已经写好了
* worker每次处理一个请求 他们的行为非常简单 如果worker崩溃了 会有另外的脚本启动他们
* 集群之间会有一个更上层的网络来连接
* 每个集群约有1000个client 单个client每秒会发送10次请求 请求包含的内容很少 应答也很少 每个不超过1KB

#### 设计
##### 单个集群的架构
    worker和client是同步的 使用LRU负载均衡多个worker
    worker是匿名的 client不直接和worker通信 因此不需要保证消息的送达以及失败后的重试

##### 多个集群的架构

###### 如何让一个集群的client和另一个集群的worker进行通信
* client直接和多个代理相连接
    优:
    劣:
* worker直接和多个代理相连接
    优:
    劣:
* 代理之间可以互相连接
    优:
    劣:

* 方案一
* 方案二

##### 代理的连接模式
    联邦模式Federation
    同伴模式Peering

##### 消息流
    状态流(state) between the broker and its peer brokers
##### 代理程序的套接字分布
##### 状态流原型
##### 本地流和云端流原型

## Reliable Request-Reply Patterns
### Client-Side Reliability (Lazy Pirate Pattern)
    REQ-REP连接模式 
        REQ带有重试机制,REQ一侧使用zmq_poll(...)设置超时时间,超时未收到回复,则关闭原来的套接字,创建新的套接字重新
    发起连接.
    lpclient.cc lpserver.cc

![lazy-pirate-pattern](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/lazy-pirate-pattern.png)

### Basic Reliable Queuing (Simple Pirate Pattern)
    REQ-[ROUTER-lruqueue-ROUTER]-REQ
    client一侧的req带有重试机制 broker作为client和server之间的代理,通过lru队列选择后端worker,通过多个worker的方
法解决了REQ-REP模式下单一REP的故障无法解决的问题,但这个引入了新的单点故障broker,且broker将请求转发给后端worker，一旦worker崩溃且未处理该请求,则该请求丢失.broker无法感知worker的状态.

    spqueue.cc spworker.cc lpclient.cc

![simple-pirate-pattern](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/simple-pirate-pattern.png)

### Robust Reliable Queuing (Paranoid Pirate Pattern)
    REQ-[ROUTER-lruqueue-ROUTER]-DEALTER
#### 实现细节
* client-REQ一侧依然是带有重试机制
* ROUTER-lruqueue-ROUTER的broker,lruqueue中存储的不再是简单的worker的identity,而是worker对象(包含identity和expiry),检测到worker过期则将worker清除,因此lruqueue中保存的始终是存活的后端worker.broker会定期的发送心跳到后端worker探测后端worker是否存活.
* worker-DEALTER一侧新增探测broker是否存活的心跳,若发现worker到broker的连接已不同,达到设定的健康心跳后,则重新建立到broker的连接.

由于worker套接字类型为DEALTER则worker需要自己处理信封envelope

    ppqueue.cc ppworker.cc lpclient.cc

![paranoid-pirate-pattern](https://github.com/96189/xteam/blob/master/OpenSource/libzmq/my-dir/paranoid-pirate-pattern.png)

    通过心跳解决了broker和worker之间无法知道对方是否存活的问题,一旦broker检测到worker死亡,则在lruqueue中删除该worker对
象;worker检测到broker死亡,由于只存在一个worker,则留给worker的选择只有重连.
    本模式并没有解决broker单点故障的问题.

#### Heartbeating for Paranoid Pirate
* worker如何探测broker(queue)的心跳
    * liveness 
        最多可错过的心跳次数,每次超时
    * zmq_poll
        以心跳间隔时间作为参数,函数返回后必然是有消息达到或者由于超时返回.
        若worker有消息达到 则将所有的有效输入数据都作为心跳对待,重置liveness值
        若是由于超时 则对liveness递减,直至达到0,重连(销毁旧的套接字建立新的套接字并重新连接),然后重置liveness值
        给broker发送心跳消息 并重置心跳开始时间

* broker(queue)如何探测worker的心跳
    * expiry 
        过期时间 = zclock_time() + HEARTBEAT_LIVENESS * HEARTBEAT_INTERVAL
    * lruqueue
        若worker有消息到达queue,则新建带有过期时间的worker对象存入lruqueue
        若client有消息到达queue,则取出并删除lruqueue队列中的第一个worker,将任务指派给该worker
        对当前lruqueue中保存的worker连接发送心跳消息 重置心跳开始时间
        清除lruqueue中连接已经过期的worker对象
    * zmq_poll
        以HEARTBEAT_INTERVAL作为参数

### 关于心跳
    心跳不是一种请求-应答 它们异步地在节点之间传递 任一节点都可以通过它来判断对方已经死亡 并中止通信
#### 为什么需要心跳
* 若application使用ROUTER套接字,对端peer发生disconnect和reconnect,则application将会发生资源泄漏
* 若application是基于SUB或DEALER套接字的数据接收者,则application无法区分当前是good silence还是bad silence,若是bad silence则application应该连接八婆备用的router
* 若application使用的是传输层协议为tcp,则tcp的心跳机制不能保证对应的应用层收到数据,因此需要应用层设计心跳协议,以确保应用层判断对方是否存活

#### 使用心跳潜在的问题
* It can be inaccurate when we send large amounts of data, as heartbeats will be delayed behind that data. If heartbeats are delayed(被延迟), you can get false timeouts and disconnections due to network congestion(网络拥塞). Thus, always treat any incoming data as a heartbeat(输入数据都作为心跳对待), whether or not the sender optimizes out heartbeats.
* While the pub-sub pattern will drop messages(丢弃消息) for disappeared recipients, PUSH and DEALER sockets will queue(排队) them. So if you send heartbeats to a dead peer and it comes back, it will get all the heartbeats you sent, which can be thousands.
* This design assumes that heartbeat timeouts are the same across the whole network. But that won't be accurate. Some peers will want very aggressive heartbeating(积极的心跳机制) in order to detect faults rapidly(迅速的检测故障). And some will want very relaxed heartbeating(轻松的心跳机制), in order to let sleeping networks lie and save power.

#### ping-pong dialog
    treat any incoming data as a pong, and only send a ping when not otherwise sending data.


规范化
抽象(中间层)
框架
https://rfc.zeromq.org/spec:6/PPP/
https://rfc.zeromq.org/spec:7/MDP/




### API
    // socket套接字
    // 数据内容起始地址
    // 有效数据实际长度(不包含'\0')
    // 
    int zmq_send (void *s_, const void *buf_, size_t len_, int flags_)

    // socket套接字
    // 缓冲区地址
    // 缓冲区大小
    // 
    int zmq_recv (void *s_, void *buf_, size_t len_, int flags_)

图片来源:http://zguide.zeromq.org/page:all