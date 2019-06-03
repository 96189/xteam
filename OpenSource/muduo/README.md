
# muduo源码学习 #
    代码中大量使用组合模式(避免OOP继承)


## 线程同步 ##

* 原子操作

    防止编译器优化的类型指示符
        volatile
    
    编译器原子操作指令

        GET -> __sync_val_compare_and_swap
        ADD -> __sync_fetch_and_add
        SET -> __sync_lock_test_and_set

* 线程锁

    RAII + pthread_mutex_XXX系列函数实现自动管理生命周期的线程锁

* 条件变量

    pthread_cond_t

* CountDownLatch

    用于线程间的同步,常用于主线程等待其他n个线程完成之后主线程才执行自己的后续任务,这是主线程与其他线程之间的执行序列同步关系.

## 阻塞队列 ##

* 为什么需要阻塞队列?

    如果生产者和消费者在某个时间段内，万一发生数据处理速度不匹配的情况呢？理想情况下，如果生产者产出数据的速度大于消费者消费的速度，并且当生产出来的数据累积到一定程度的时候，那么生产者必须暂停等待一下（阻塞生产者线程），以便等待消费者线程把累积的数据处理完毕，反之亦然。然而，在concurrent包发布以前，在多线程环境下，我们每个程序员都必须去自己控制这些细节，尤其还要兼顾效率和线程安全，而这会给我们的程序带来不小的复杂度。好在此时，强大的concurrent包横空出世了，而他也给我们带来了强大的BlockingQueue。（在多线程领域：所谓阻塞，在某些情况下会挂起线程（即阻塞），一旦条件满足，被挂起的线程又会自动被唤醒）

    作为BlockingQueue的使用者，我们再也不需要关心什么时候需要阻塞线程，什么时候需要唤醒线程，因为这一切BlockingQueue都给你一手包办了。

* BlockingQueue

    queue + mutex + condition(不空的条件变量)

* BoundedBlockingQueue(限制大小的阻塞队列)

由于限制了队列大小 需要不满的条件变量来防止数据插入造成队列满
    queue + mutex + condition(不空的条件变量) + condition(不满的条件变量)


## 线程池 ##

* 线程创建

    c的pthread_create函数

* 线程私有数据(线程函数参数)ThreadData

    线程私有数据整个线程内具有全局作用域,各线程有各自的实体
    __thread关键字表示线程私有数据
    pthread_key_create函数创建kv结构的线程私有数据

* 线程池

    线程锁

        同一时刻是有一个线程向线程池添加任务

    线程池数组

        每个线程在线程池队列上阻塞等待拿到任务

    任务队列

        生产者消费者问题 队列空时阻塞消费者  队列满时阻塞生产者

    队列条件变量

        满条件变量 空条件变量

    线程执行函数

        阻塞在任务队列等待获取任务执行

* QA

    typedef boost::function<void ()> Task;任务参数如何传递?

## muduo NIO ##
    类似java NIO

* channel

    仅仅指的是TCP网络通道,封装套接字描述符fd,任务处理框架(reactor)loop_,回调函数readCallback_,writeCallback_,closeCallback_,errorCallback_.

* poller

    I/O复用接口类,持有std::map<int, Channel*> channels_,TCP通道列表.归属于某一个务处理框架(reactor)ownerLoop_,接口中主要定义了poll方法.
    
    EPollPoller和PollPoller是I/O复用的实例,muduo中使用继承关系的一处.

* eventloop

    reactor,每线程一个reactor.持有I/O复用实例poller_,活跃的channel列表activeChannels_,定时器列表timerQueue_

* buffer

    std::vector<char> buffer_的封装,提供数据读写等接口操作.

## 服务器模式 ##
    类似ACE的Acceptor-Connector模式

* Acceptor

    TCP连接接收器,持有acceptSocket_对象,acceptChannel_通道,reactor处理框架loop_以及提供外部注册新连接处理函数newConnectionCallback_.

* TcpConnection

    TCP连接对象,持有socket_对象,channel_连接通道,reactor处理框架loop_,输入输出缓冲区inputBuffer_/outputBuffer_,连接建立回调connectionCallback_,消息到达回调messageCallback_,写完成回调writeCompleteCallback_,到达高水位标记回调highWaterMarkCallback_,连接关闭回调closeCallback_.

* TcpServer

    持有连接接收器acceptor_和std::map<string, TcpConnectionPtr> connections_已建立连接的表.由外部提供的loop_,提供连接建立,消息到达,写完成回调设置接口,外部设置成功后,由TcpServer内部在建立新连接的时候将这些回调设置给TcpConnection.

    持有每线程一个reactor的线程池threadPool_,每新建立一个连接则在线程池中选择一个loop(reactor).TcpServer::newConnection函数中获取loop 每连接一个loop(reactor) 每个loop由一个线程创建 获取到一个loop等价于获取到一个线程 

* EventLoopThreadPool每线程一个loop(reactor)的线程池

1. EventLoopThreadPool
    
    内部持有外部提供的reactor EventLoop* baseLoop_,保存线程的列表boost::ptr_vector<EventLoopThread> threads_,保存loop(reactor)的列表std::vector<EventLoop*> loops_,创建线程池时生成线程,线程函数threadFunc执行时创建loop.本线程池提供轮询和哈希两种方式获取loop(即获取线程).

2. EventLoopThread

    内部持有线程对象Thread thread_和reactor的指针EventLoop* loop_,线程函数执行时实例化reactor对象,使每线程一个reactor。
    ```
    void EventLoopThread::threadFunc()
    {
    EventLoop loop;

    if (callback_)
    {
        callback_(&loop);
    }

    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;  // 指针指向实例化后的reactor
        cond_.notify();
    }

    loop.loop();    // 循环阻塞

    MutexLockGuard lock(mutex_);
    loop_ = NULL;
    }
    ```

## 应用实例 ##
    ChargenServer

## 定时器 ## 


## EventLoop::runInLoop ##

