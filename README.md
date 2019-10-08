
## 一、编程语言
### CPP
+   STL 
    *  [map中key的排序方式? (less(key))](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_map.h)
    *  [vector写满以后底层如何处理? (x2)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_vector.h)
    *  [STL第二级空间适配器 (数组+自由链表)](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/stl_alloc.h)

+   对象模型
    *  是否能在构造函数中调用memset(this, sizeof(this), 0)将对象清零?(虚函数与虚表)

+   智能指针

    自动管理资源(内存、文件、连接)的生命周期,对原始资源封装一层,通过RAII利用构造和析构函数自动申请和释放资源.需要注意的是拷贝构造函数与赋值运算符的重载函数的实现,是要public还是private,这决定了资源的所有权是否可以转移以及可能导致的重复释放及空悬指针问题.但明显的一点是对于资源不应该依赖编译器默认生成的拷贝构造和赋值运算符重载函数.

    *   [auto_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/backward/auto_ptr.h)
        
        由于可进行资源所有权的转移,所以拷贝构造函数与赋值运算符重载函数设置为public,转移意味着解除旧的对象与资源的绑定关系,由新的对象来管理资源,旧的对象不再可用,一旦再次误用旧的对象可能导致空指针解引用.此外若误用将两个智能指针管理同一个资源,其中一个释放将导致另一个为空悬指针.由于资源所有权的转移特性使得不满足容器性质,不能存放在容器中.不能用来管理一组资源,由于释放资源的时候使用的是delete而不是delete[].

        造成如上问题的大部分原因在于所有权转移的特性,使得拷贝构造函数和赋值运算符重载实现为public造成资源管理的困难.

    *   [unique_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/unique_ptr.h)

        资源所有权的唯一性,同一时刻只有一个对象可以管理资源.类实现时将拷贝构造函数和赋值运算符重载函数标记为delete,禁止资源所有权转移.引入右值引用和完美转发,实现移动构造函数和移动赋值运算符重载.unique_ptr实现了可管理数组资源的版本

        如果误用,同一时刻将两个智能指针对象管理同一个资源,unique_ptr依然会导致程序异常.

    *   [shared_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

        共享资源声明周期,引入__shared_count

    *   [weak_ptr](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/STL/4.8.2/bits/shared_ptr_base.h)

        观察资源是否已被释放,引入__weak_count

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

## 二、算法
+   排序与查找
    *   快速排序
    *   归并排序
    *   堆排序
    *   选择排序
    *   插入排序
    *   冒泡排序
    *   二分查找

+ 图算法
    *   DFS
    *   BFS
+ 递归与分治策略
    *   [汉诺塔](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [斐波那契数列](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [字符串全排列](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [二分查找](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [归并排序](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [快速排序](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [线性选择](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)
    *   [最接近点对](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/recursive.go)

+ 动态规划
    *   [斐波那契数列](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/dp.go)
    *   [硬币组合-不限硬币个数](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/dp.go)
    *   [最长非降序子序列](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/dp.go)
    *   [最大连续子序列和](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/dp.go)
    *   [最长公共子序列](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/dp.go)
    *   [0-1背包](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/dp.go)


+   贪心策略
    *   [硬币组合-不限硬币个数](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   [硬币组合-限定每种硬币个数](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   [活动安排](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   [BestCowLine](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   [均分纸牌](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   [最大整数](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   [最优装载](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/greedy.go)
    *   背包问题
    *   旅行商问题
    *   哈夫曼编码
    *   最小生成树
    *   最短路径

+   回溯
    *   [n皇后问题](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/backtracking.go)
    *   [全排列问题](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/backtracking.go)
    *   [电话号码的字母组合](https://github.com/96189/xteam/blob/master/%E7%AE%97%E6%B3%95/go/backtracking.go)

+   其他
    *   shuffle实现(随机化)
    *   红包分配算法m元钱n个人分完,分得的数值随机


## 三、数据结构
+   跳跃表
    *   如何加快查询以及在redis中的应用?
+   红黑树
    *   epoll对于事件的管理方式,nginx对于定时器的管理方式,STL中map和set的底层实现
+   最大/小堆
    *   top N问题

+   bitmap

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

+   I/O复用
    *   select(查询式)
    *   epoll(启发式)
        *   ET与LT区别及编程注意事项
    *   对比

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
        *  [缓存淘汰策略(LRU/LFU)](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/%E5%BC%80%E6%BA%90/Redis/redis-cache-expired.md)
        *  [主从复制](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/%E5%BC%80%E6%BA%90/Redis/redis-master-slave.md)
        *  [数据持久化RDB/AOF](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/%E5%BC%80%E6%BA%90/Redis/redis-persistence.md)
        *  如何保证高可用?
        *  集群数据一致性如何保证?

## 七、分布式系统

+   消息中间件
    *   [zmq](https://github.com/96189/xteam/tree/master/%E5%BC%80%E6%BA%90/libzmq/my-dir)

+   服务协同

    多个进程节点作为整体对外提供服务,服务可以相互发现,服务关注者可以及时获取被关注者的变化以完成协作。具体运行过程包括:服务注册和服务发现。
        
    *   服务注册与服务发现
    *   统一命名

            对服务以及其中的节点，进行集中式、统一命名，便于相互区分和访问。

    *   监控

            确定服务的可用性和状态，当服务状态变化时，关注者要有途径获知。

        *   健康检查(心跳机制)

    *   访问策略

        服务通常包含多个节点，以集群形式存在，Client在每次请求时需要策略确定通信节点，策略目标可能是多样的，比如 负载均衡 ，稳定映射 等等。

        *   负载均衡

            *   [常见负载均衡算法及应用场景](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/Nginx/src/ngx_load_balance.md)

    *   可用性

        容灾处理，动态扩容。

        *   单点故障、容灾、容错、数据一致性

        *   进程挂掉如何容灾、单点故障如何解决

+   微服务

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
