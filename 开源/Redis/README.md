redis源码版本:
Redis server v=4.0.10 sha=00000000:0 malloc=libc bits=64 build=d68c5d3f7b8aefc2

[redis为什么要设计成单线程](https://www.modb.pro/db/32089)

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

## 场景应用
* [并发操作同一个redis key](https://cloud.tencent.com/developer/article/1536305)

    * 乐观锁 

        watch 监听key

        multi 开启事务

        ... 执行命令
        
        exec 提交事务 (提交事务时，watch监听的key若被修改过，本次提交不生效)

    
    * 分布式锁

        SETNX key value     // 『SET if Not eXists』(如果不存在，则 SET)

        SET key value EX seconds NX (key不存在时设置value并且设置过期时间为ex秒)

    * 时间戳(适合有序场景)

        存储时带上时间戳

    * 消息队列(适合分布式系统环境)

        利用消息中间件串行化


## sentinel

    sentinel解决HA(高可用)问题,实现监控、通知、自动故障恢复


## cluster

    cluster解决sharding(分片)问题,满足扩展性(分布式存储)

+    1、redis集群方案?

+    2、Redis集群方案什么情况下会导致整个集群不可用？

+    3、Redis哈希槽的概念？

##  [我能想到关于redis的事情](https://blog.csdn.net/a1290123825/article/details/89008067#%E5%A6%82%E6%9E%9C%E6%9C%89%E5%A4%A7%E9%87%8F%E7%9A%84-key-%E9%9C%80%E8%A6%81%E8%AE%BE%E7%BD%AE%E5%90%8C%E4%B8%80%E6%97%B6%E9%97%B4%E8%BF%87%E6%9C%9F%EF%BC%8C%E4%B8%80%E8%88%AC%E9%9C%80%E8%A6%81%E6%B3%A8%E6%84%8F%E4%BB%80%E4%B9%88%EF%BC%9F)

## redis不停机在线数据迁移
* 适用于直接连接redis实例，redis无proxy的情况

    redis-cli -h {ip} -p {port} --pipe < appendonly.aof

* 适用于redis proxy twemproxy的情况

    https://github.com/vipshop/redis-migrate-tool

* 其他工具

    https://blog.csdn.net/cron_zzx/article/details/87362171

* 其他文档

    https://shimo.im/docs/WrPwKQcKHXxJYD8Y/read