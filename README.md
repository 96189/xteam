
## 一、编程
### 编程思想

+   OOP & OOD & SOLID

    *   单一指责原则(Single Responsibility)

            每个类应该只有一个目的。如果类A有两个目的，那么分别创建类B和类C来处理每个目的，再从B和C中提炼出A。

    *   开放封闭原则(Open / Closed)

            软件随时都在变化（即维护很重要）。把可能会变化的部分放到实现（即具体的类）中，给不太可能会变化的东西建立接口（比如抽象基类）。

    *   里氏替换原则(Liskov Substitution)   -   防止滥用继承

            “继承必须确保超类所拥有的性质在子类中仍然成立。”也就是说，当一个子类的实例应该能够替换任何其超类的实例时，它们之间才具有is-A关系。

    *   接口隔离原则(Interface Segregation)

            接口应当尽量小，保证每一部分代码都“只需了解”最小量的代码，也就是说避免不必要的依赖。这一条建议对C++也很好用，因为不遵循这条原则会让编译时间大幅增长。

    *   依赖倒置原则(Dependency Inversion)  -   面向接口编程

            程序要依赖于抽象接口，不要依赖于具体实现。简单的说就是要求对抽象进行编程，不要对实现进行编程，这样就降低了客户与实现模块间的耦合。



### CPP
+   STL 
    *  [map中key的排序方式? (less(key))](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_map.h)
    *  [vector写满以后底层如何处理? (x2)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_vector.h)
    *  [STL第二级空间适配器 (数组+自由链表)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_alloc.h)

+   对象模型
    *  是否能在构造函数中调用memset(this, sizeof(this), 0)将对象清零?(虚函数与虚表)
    *  如何实现一个不能被继承的类(FinalClass)

+   [智能指针](https://github.com/96189/xteam/blob/master/%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80/cpp/README.md)
    *   [auto_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/backward/auto_ptr.h)

    *   [unique_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/unique_ptr.h)

    *   [shared_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    *   [weak_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

    *   boost::scoped_ptr
+   bind & function & lambd
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
    *  三次握手
    *  四次挥手
    *  TIME_WAIT与2MSL
    *  SO_LINGER、SOL_SOCKET、SO_KEEPALIVE选项
    *  netstat、ss命令

+   UDP
+   HTTP
    *   session和cookie的区别
+   HTTPS

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
