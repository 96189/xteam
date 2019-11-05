redis源码版本:
Redis server v=4.0.10 sha=00000000:0 malloc=libc bits=64 build=d68c5d3f7b8aefc2

## redis常见问题
+    &radic;1、[假如Redis里面有1亿个key，其中有10w个key是以某个固定的已知的前缀开头的，如果将它们全部找出来？](http://doc.redisfans.com/key/scan.html)


+    &radic;2、如果有大量的key需要设置同一时间过期，一般需要注意什么？

+    &radic;3、[Pipeline(批量发送请求)有什么好处，为什么要用pipeline？](https://redis.io/topics/pipelining)
    
+    &radic;4、redis分布式锁?

        *   单节点情况下set指令

            尝试获取锁:SET key value EX seconds NX (key不存在时设置value并且设置过期时间为ex秒)

        *   多节点情况下redlock
        
            https://redis.io/topics/distlock

            http://zhangtielei.com/posts/blog-redlock-reasoning.html

    
+    &radic;5、[如何用redis实现异步队列?](https://github.com/antirez/disque)
    
        *   List

            入队:RPUSH key value [value ..] 
            
            出队:LPOP key

            可能存在等待队列中没值就消费的问题,缓解策略是在应用中定期LPOP尝试

            出队:BLPOP key [key ...] timeout

            若队列中无值则阻塞队列,只能供一个消费者消费

        *   Pub/Sub

            订阅:SUBSCRIBE channel [channel ...]

            发布:PUBLISH channel message

            发布订阅模式无状态且消息没有持久化,也没有确认收到的机制,若订阅方离线或者网络原因,消息可能无法送达
    
+    &radic;6、[如何用redis实现延时队列?](https://tech.youzan.com/queuing_delay/)

        SortedSet有序列表,按照score排序.

        添加:ZADD key score member [[score member] [score member] ...]

        排序:ZREMRANGEBYSCORE key min max (获取某一个时间之前所有到期消息)

        移除:ZREM key member [member ...]

        member是唯一的,如果member已存在则更新member的score值.
        
        延迟队列    以过期时间为score,以消息Id为member.
        
        消息池  以hash结构存储消息id和消息内容

        定时任务    扫描并处理过期消息.

+    &radic;7、[redis如何做持久化(RDB AOF)?](http://doc.redisfans.com/topic/persistence.html#redis)

+    8、Redis主从复制与读写分离？

+    9、redis的同步机制?如何做到主从一致性?会不会存在master写成功,slave写失败的情况?

+    10、Redis常见的性能问题都有哪些？如何解决？


## sentinel

    sentinel解决HA(高可用)问题,实现监控、通知、自动故障恢复


## cluster

    cluster解决sharding(分片)问题,满足扩展性(分布式存储)

+    1、redis集群方案?

+    2、Redis集群方案什么情况下会导致整个集群不可用？

+    3、Redis哈希槽的概念？
