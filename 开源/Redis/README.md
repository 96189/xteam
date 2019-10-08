redis源码版本:
Redis server v=4.0.10 sha=00000000:0 malloc=libc bits=64 build=d68c5d3f7b8aefc2

# redis常见问题
    1、假如Redis里面有1亿个key，其中有10w个key是以某个固定的已知的前缀开头的，如果将它们全部找出来？
    
    2、如果有大量的key需要设置同一时间过期，一般需要注意什么？
    
    3、Pipeline(批量发送请求)有什么好处，为什么要用pipeline？
        https://redis.io/topics/pipelining
    
    4、redis分布式锁?
        https://redis.io/topics/distlock
        http://zhangtielei.com/posts/blog-redlock-reasoning.html
    
    5、如何用redis实现异步队列?
        https://github.com/antirez/disque
    
    6、如何用redis实现延时队列?

    7、redis的同步机制?Redis集群的主从复制模型是怎样的？

    8、redis如何做持久化?

    9、redis集群方案?

    10、MySQL里有2000w数据,redis中只存20w的数据,如何保证redis中的数据都是热点数据

    11、Redis常见的性能问题都有哪些？如何解决？

    12、redis最适合的场景

    13、Redis集群方案什么情况下会导致整个集群不可用？

    14、Redis哈希槽的概念？

    15、sentinel和cluster的区别
        sentinel解决HA(高可用)问题,实现监控、通知、自动故障恢复,cluster解决sharding(分片)问题,满足扩展性

    16、如何做到将redis读请求分配到只读的slave节点 写请求分配到master节点?

    17、redis如何做到主从一致性?会不会存在master写成功,slave写失败的情况?