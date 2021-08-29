
## 一、编程
### CPP
+   STL 
    *  [map中key的排序方式? (less(key))](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_map.h)
    *  [vector写满以后底层如何处理? (x2)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_vector.h)
    *  [STL第二级空间适配器 (数组+自由链表)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_alloc.h)

+   对象模型
    *  是否能在构造函数中调用memset(this, sizeof(this), 0)将对象清零?(虚函数与虚表)
    *  如何实现一个不能被继承的类(FinalClass)
    *  c++中空对象占多大空间?为什么空对象要占空间?(1byte 两个不一样的对象要有不一样的地址 类对象可作为数组元素 如果是0没法存储)
    *   一个类中最多只允许一个虚函数表吗?

+   [智能指针](https://github.com/96189/xteam/blob/master/%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80/cpp/README.md)
    *   [auto_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/backward/auto_ptr.h)

    *   [unique_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/unique_ptr.h)

    *   [shared_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    *   [weak_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    *   boost::scoped_ptr
+   bind & function & lambda

    c函数指针(回调) -> c++ std::function

    std::bind函数适配器,生成一个新的可调用对象来“适应”原对象的参数列表

    lambda 匿名函数 =按值传递 &按引用传递

+   c++11
    *   右值引用
    *   移动语义
    *   完美转发
+   boost

### GO
+   语法
    *  make和new的区别

## 二、[算法](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)
+   [排序与查找](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)

+   [图算法](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)

+   [递归与分治策略](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)

+   [动态规划](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)

+   [贪心策略](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)

+   [回溯](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)

+   [其他](https://github.com/96189/xteam/tree/master/%E7%AE%97%E6%B3%95)


## 三、数据结构
+   跳跃表
    *   如何加快查询以及在redis中的应用?
+   红黑树
    *   epoll对于事件的管理方式,nginx对于定时器的管理方式,STL中map和set的底层实现
+   最大/小堆
    *   top N问题

+   bitmap

    *   Bloom filter

+   [B-树](https://github.com/96189/xteam/blob/master/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/README.md)

    *   MongoDB存储引擎索引

+   [B+树](https://github.com/96189/xteam/blob/master/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/README.md)

    *   [为什么mysql存储引擎索引使用B+树](https://github.com/96189/xteam/blob/master/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84/README.md)

+   LSM树
    
    *   MongoDB的存储引擎索引

## 四、协议
### 网络协议
+   TCP
    *  连接建立(三次握手)的过程及状态转移

        ```
        C:syn    -> S:ack+syn          -> C:ack
        C:SYN-SENT  S:LISTEN  S:SYN_RECV  C:ESTABLISH  S:ESTABLISH
        ```
    *  为什么建立连接需要3次,2次行不行?

        2次可能形成死锁,假设2次就建立连接,S:ack后认为连接成功建立.此时该ack丢失C没有收到.从此刻开始S发数据到C,等待C收到数据后发来ack,此时C还在等待握手的ack,C和S由于丢失了S的ack以后互相等待.

    *  连接断开(四次挥手)的过程及状态转移

        ```
        C:fin     -> S:ack                  -> S:fin   -> C:ack
        C:FIN_WAIT_1 S:CLOSE_WAIT C:FIN_WAIT_2 S:LAST_ACK C:TIME_WAIT S:CLOSED C:CLOSED
        ```
    *  为什么连接断开需要4次,和连接建立相比多了哪一次?

        断开连接的4次和建立连接的3次对比,多1次是因为建立连接时服务器将ack+syn在一个数据包中发送,但是断开连接时ack和fin是分两次发送的.之所以分两次是因为tcp是全双工的,一方主动关闭,需要给对方一段时间发送剩余数据,然后对方发fin再关闭,ack和fin之间的时间间隔就是等待被动关闭一方发送最后数据的时间.主动断开的一方发fin并收到对端回复的ack后进入半关闭状态.

    *  TIME_WAIT与2MSL

        主动关闭的一方进入TIME_WAIT状态,TIME_WAIT的存在是为了防止主动关闭的一方最后发送的ack丢失,如果丢失那么会再次收到被动关闭的一方发来的fin,重新计时2MSL,如果登录2MSL还没收到对方的FIN发过来那么就假设ack被对方收到,主动断开的一方进入关闭状态.

    *  SO_LINGER、SO_REUSEADDR、SO_KEEPALIVE选项
    *  netstat、ss命令

+   UDP
+   HTTP
    *   session和cookie的区别
+   HTTPS
    *   https密钥协商过程

### 应用协议
+   Protobuff
    
    *   [数据编码](https://www.jianshu.com/p/73c9ed3a4877)

        Tag - [Length] - Value编码结构,当Value 采用 Varints 编码时,不需要Length变为Tag - Value编码结构.Tag 由 field_number 和 wire_type 两个部分组成,field_number: message 定义字段时指定的字段编号,wire_type:ProtoBuf 编码类型，根据这个类型选择不同的 Value 编码方案。

        TLV编码结构的数据类型:string、bytes、EmbeddedMessage、repeated

        TV编码结构的数据类型:int32、int64、uint32、uint64、bool、enum、fixed64、sfixed64、double(Varints 编码)、sint32、sint64(ZigZag 编码)

        Varints编码:在每个字节开头的 bit 设置了 msb(most significant bit ),标识是否需要继续读取下一个字节


## 五、网络编程
+   阻塞、非阻塞、同步、异步

+   [I/O复用](https://github.com/96189/xteam/blob/master/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/README.md)
    *   [select](https://github.com/96189/xteam/blob/master/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/README.md)
    *   [epoll](https://github.com/96189/xteam/blob/master/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/README.md)
    *   [对比](https://github.com/96189/xteam/blob/master/%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B/README.md)

+   事件驱动Reactor


## 六、存储
+   SQL
    *   MySQL
        *   存储引擎
        *   索引(快速查询)
        *   读写分离
        *   分表
+   NOSQL
    *   Redis
        *  [缓存淘汰策略(LRU/LFU)](https://github.com/96189/xteam/tree/master/%E5%BC%80%E6%BA%90/Redis/redis-cache-expired.md)
        *  [主从复制](https://github.com/96189/xteam/tree/master/%E5%BC%80%E6%BA%90/Redis/redis-master-slave.md)
        *  [数据持久化RDB/AOF](https://github.com/96189/xteam/tree/master/%E5%BC%80%E6%BA%90/Redis/redis-persistence.md)
        *  如何保证高可用?
        *  集群数据一致性如何保证?

## 七、分布式系统

+   [CAP理论](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [BASE理论](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [分布式系统的特性](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [分布式系统面临的主要问题](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [分布式系统中具体的细节](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [分布式系统实践](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [分布式系统的常用组件](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   [微服务](https://github.com/96189/xteam/tree/master/%E5%88%86%E5%B8%83%E5%BC%8F%E7%B3%BB%E7%BB%9F/README.md)
+   应用
    *   [分布式ID生成系统](https://tech.meituan.com/2017/04/21/mt-leaf.html)

## 八、设计模式

+   创建型模式
    *   [抽象工厂模式](https://github.com/96189/xteam/tree/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F/Factory/AbstractFactory)
    *   建造者模式
    *   [工厂方法模式](https://github.com/96189/xteam/tree/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F/Factory/FactoryMethod)
    *   原型模式
    *   [单例模式](https://github.com/96189/xteam/blob/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F/Singleton/singleton.hpp)

+   结构性模式
    *   [适配器模式](https://github.com/96189/xteam/tree/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E7%BB%93%E6%9E%84%E5%9E%8B%E6%A8%A1%E5%BC%8F/Adapter)
    *   桥接模式
    *   组合模式
    *   装饰模式
    *   [外观模式](https://github.com/96189/xteam/tree/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E7%BB%93%E6%9E%84%E5%9E%8B%E6%A8%A1%E5%BC%8F/Facade)
    *   享元模式
    *   [代理模式](https://github.com/96189/xteam/tree/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E7%BB%93%E6%9E%84%E5%9E%8B%E6%A8%A1%E5%BC%8F/Proxy)

+   行为模式
    *   职责链模式
    *   命令模式
    *   解释器模式
    *   迭代器模式
    *   中介者模式
    *   备忘录模式
    *   [观察者模式](https://github.com/96189/xteam/tree/master/%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F/%E8%A1%8C%E4%B8%BA%E6%A8%A1%E5%BC%8F/Observer)
    *   状态模式
    *   策略模式
    *   模板方法模式
    *   访问者模式

## 九、系统设计
+ 游戏排行榜(桶排序、跳跃表)
+ 游戏匹配


## 十、架构设计与系统优化、系统性能瓶颈分析
+ 内存优化
    +   [nginx的内存池设计](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/Nginx/src/nginx_mempool.pdf)
    +   [redis数据结构设计](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/Redis/redis%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84.pdf)

+ 架构设计
    *   [nginx的master-slave模型]()

## 十一、测试调试及优化
+   测试
    *   单元测试
        *   gtest

+   调试与coredump分析
    *   GDB

+   性能分析
    *   PProf
    *   Valgrind

## 十二、操作系统
+   进程
+   线程
+   协程
+   系统调度算法
+   linux内存管理机制

    *   关键词

        物理内存与虚拟内存及内存地址映射

        交换空间

        分页机制与页面替换

    *   进程内存区域分布模型

        ![Linux进程在虚拟内存中的标准内存段布局](./%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F/img/linux-process-space.jpg)

    *   进程内存的分配与回收

        do_mmap()函数

        函数do_ummap()

## 十三、开发工具
+   版本控制工具
    *   GIT
    *   SVN
+   VIM
    * 配置



## 其他
+   无锁队列、环形缓冲区

+   进程间通信

+   [后端架构师技术图谱](https://github.com/xingshaocheng/architect-awesome)
+   [系统设计入门](https://github.com/donnemartin/system-design-primer/blob/master/README-zh-Hans.md)
+   [远程工作](https://github.com/greatghoul/remote-working)
+   [distribute_crawler](https://github.com/gnemoug/distribute_crawler)
+   [free-programming-books](https://github.com/EbookFoundation/free-programming-books/blob/master/free-programming-books-zh.md)
