
## 一、编程语言
### cpp
*   STL 
    1.  map中key的排序方式?
    2.  vector写满以后底层如何处理?
    3.  STL第二级空间适配器(数组+自由链表)

*   虚函数与虚表
    1.  是否能在构造函数中调用memset(this, sizeof(this), 0)将对象清零?

### go
*   语法
    1.  make和new的区别

## 二、算法
### 排序
*   快速排序
*   归并排序
*   堆排序
*   选择排序
*   插入排序
*   冒泡排序

### 搜索
*   二分查找
*   DFS
*   BFS

### 其他
*   shuffle实现(随机化)
*   红包分配算法m元钱n个人分完,分得的数值随机

### 递归与分治策略
### 动态规划
### 贪心策略
### 回溯

## 三、数据结构
### 跳跃表
*   如何加快查询以及在redis中的引用?

## 四、协议
### 网络协议
*   TCP
    1.  三次握手
    2.  四次挥手
    3.  TIME_WAIT与2MSL
    4.  SO_LINGER、SOL_SOCKET、SO_KEEPALIVE选项
    5.  netstat、ss命令

*   UDP
*   HTTP
*   HTTPS

### 应用协议
*   Protobuff


## 五、网络编程
### 阻塞、非阻塞、同步、异步

### I/O复用
*   select(查询式)
*   epoll(启发式)
    1.  ET与LT区别及编程注意事项
*   对比

### 事件驱动Reactor


## 六、存储
### SQL
*   MySQL
    1.  存储引擎
    2.  索引(快速查询)

### NOSQL
*   Redis
    1.  [缓存淘汰策略(LRU/LFU)](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/OpenSource/Redis/redis-cache-expired.md)
    2.  [主从复制](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/OpenSource/Redis/redis-master-slave.md)
    3.  [数据持久化RDB/AOF](https://github.com/96189/xteam/blob/16d1050de8596ef38d495b52de7961bd2747d516/OpenSource/Redis/redis-persistence.md)
    4.  如何保证高可用?
    5.  集群数据一致性如何保证?

## 七、分布式系统
### 负载均衡
* [常见负载均衡算法及应用场景](https://github.com/96189/xteam/blob/master/OpenSource/Nginx/src/ngx_load_balance.md)

### 消息中间件
*   [zmq](https://github.com/96189/xteam/tree/master/OpenSource/libzmq/my-dir)

### 心跳机制

### 健康检查

### 单点故障、容灾、容错、数据一致性

### 进程挂掉如何容灾、单点故障如何解决

### 微服务

### 应用
*   [分布式ID生成系统](https://tech.meituan.com/2017/04/21/mt-leaf.html)

## 八、设计模式

## 九、系统设计
* 游戏排行榜(桶排序、跳跃表)
* 游戏匹配


## 十、架构设计与系统优化、系统性能瓶颈分析
### 内存优化
*   [nginx的内存池设计](https://github.com/96189/xteam/blob/master/OpenSource/Nginx/src/nginx_mempool.pdf)
*   [redis数据结构设计](https://github.com/96189/xteam/blob/master/OpenSource/Redis/redis%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84.pdf)

### 架构设计
*   [nginx的master-slave模型]()


## 其他
*   无锁队列、环形缓冲区

*   进程间通信