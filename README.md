
## 一、编程语言
### CPP
+   STL 
    *  map中key的排序方式?
    *  vector写满以后底层如何处理?
    *  STL第二级空间适配器(数组+自由链表)

+   虚函数与虚表
    *  是否能在构造函数中调用memset(this, sizeof(this), 0)将对象清零?

### GO
+   语法
    *  make和new的区别

## 二、算法
### 排序
+   快速排序
+   归并排序
+   堆排序
+   选择排序
+   插入排序
+   冒泡排序

### 搜索
+   二分查找
+   DFS
+   BFS

### 其他
+   shuffle实现(随机化)
+   红包分配算法m元钱n个人分完,分得的数值随机

### 递归与分治策略
### 动态规划
### 贪心策略
### 回溯

## 三、数据结构
### 跳跃表
+   如何加快查询以及在redis中的引用?
### 红黑树
+   epoll对于事件的管理方式,nginx对于定时器的管理方式,STL中map和set的底层实现.
### 最大/小堆
+   top N问题

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
### 阻塞、非阻塞、同步、异步

### I/O复用
+   select(查询式)
+   epoll(启发式)
    *  ET与LT区别及编程注意事项
+   对比

### 事件驱动Reactor


## 六、存储
### SQL
+   MySQL
    *   存储引擎
    *   索引(快速查询)
    *   读写分离
    *   分表

### NOSQL
+   Redis
    *  [缓存淘汰策略(LRU/LFU)](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/OpenSource/Redis/redis-cache-expired.md)
    *  [主从复制](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/OpenSource/Redis/redis-master-slave.md)
    *  [数据持久化RDB/AOF](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/OpenSource/Redis/redis-persistence.md)
    *  如何保证高可用?
    *  集群数据一致性如何保证?

## 七、分布式系统

### 消息中间件
+   [zmq](https://github.com/96189/xteam/tree/master/OpenSource/libzmq/my-dir)

### 服务协同

    多个进程节点作为整体对外提供服务,服务可以相互发现,服务关注者可以及时获取被关注者的变化以完成协作。具体运行过程包括:服务注册和服务发现。

+   服务注册与服务发现
+   统一命名

    对服务以及其中的节点，进行集中式、统一命名，便于相互区分和访问。

+   监控

    确定服务的可用性和状态，当服务状态变化时，关注者要有途径获知。

    *   健康检查(心跳机制)

+   访问策略

    服务通常包含多个节点，以集群形式存在，Client在每次请求时需要策略确定通信节点，策略目标可能是多样的，比如 负载均衡 ，稳定映射 等等。

    *   负载均衡
        *   [常见负载均衡算法及应用场景](https://github.com/96189/xteam/blob/master/OpenSource/Nginx/src/ngx_load_balance.md)

+   可用性

    容灾处理，动态扩容。

    *   单点故障、容灾、容错、数据一致性

    *   进程挂掉如何容灾、单点故障如何解决

### 微服务

### 应用
+   [分布式ID生成系统](https://tech.meituan.com/2017/04/21/mt-leaf.html)

## 八、设计模式

    参考<<c++API设计>>

+   创建型模式
    *   [抽象工厂模式](https://github.com/96189/xteam/tree/master/designPatterns/%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F/Factory/AbstractFactory)
    *   建造者模式
    *   [工厂方法模式](https://github.com/96189/xteam/tree/master/designPatterns/%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F/Factory/FactoryMethod)
    *   原型模式
    *   [单例模式](https://github.com/96189/xteam/blob/master/designPatterns/%E5%88%9B%E5%BB%BA%E5%9E%8B%E6%A8%A1%E5%BC%8F/Singleton/singleton.hpp)

+   结构性模式
    *   [适配器模式](https://github.com/96189/xteam/tree/master/designPatterns/%E7%BB%93%E6%9E%84%E5%9E%8B%E6%A8%A1%E5%BC%8F/Adapter)
    *   桥接模式
    *   组合模式
    *   装饰模式
    *   [外观模式](https://github.com/96189/xteam/tree/master/designPatterns/%E7%BB%93%E6%9E%84%E5%9E%8B%E6%A8%A1%E5%BC%8F/Facade)
    *   享元模式
    *   [代理模式](https://github.com/96189/xteam/tree/master/designPatterns/%E7%BB%93%E6%9E%84%E5%9E%8B%E6%A8%A1%E5%BC%8F/Proxy)

+   行为模式
    *   职责链模式
    *   命令模式
    *   解释器模式
    *   迭代器模式
    *   中介者模式
    *   备忘录模式
    *   [观察者模式](https://github.com/96189/xteam/tree/master/designPatterns/%E8%A1%8C%E4%B8%BA%E6%A8%A1%E5%BC%8F/Observer)
    *   状态模式
    *   策略模式
    *   模板方法模式
    *   访问者模式

## 九、系统设计
+ 游戏排行榜(桶排序、跳跃表)
+ 游戏匹配


## 十、架构设计与系统优化、系统性能瓶颈分析
### 内存优化
+   [nginx的内存池设计](https://github.com/96189/xteam/blob/master/OpenSource/Nginx/src/nginx_mempool.pdf)
+   [redis数据结构设计](https://github.com/96189/xteam/blob/master/OpenSource/Redis/redis%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84.pdf)

### 架构设计
+   [nginx的master-slave模型]()

## 十一、工具
### GDB
### PProf

## 其他
+   无锁队列、环形缓冲区

+   进程间通信


[相关连接](https://dbaplus.cn/news-159-512-1.html)