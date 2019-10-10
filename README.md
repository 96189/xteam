
## 一、编程语言
### CPP
+   STL 
    *  [map中key的排序方式? (less(key))](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_map.h)
    *  [vector写满以后底层如何处理? (x2)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_vector.h)
    *  [STL第二级空间适配器 (数组+自由链表)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_alloc.h)

+   对象模型
    *  是否能在构造函数中调用memset(this, sizeof(this), 0)将对象清零?(虚函数与虚表)

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

## 十三、开发工具
+   版本控制工具
    *   GIT
    *   SVN
+   VIM
    * 配置



## 其他
+   无锁队列、环形缓冲区

+   进程间通信
