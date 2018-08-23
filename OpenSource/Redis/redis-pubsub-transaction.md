# 订阅与发布
## 一、底层数据结构
    struct redisServer
    {
        ...
        // 键 => 频道
        // 值 => 订阅该频道的客户端链表
        dict *pubsub_channels;
        // 所有客户端订阅的模式都存储在这里的链表中(链表中存储pubsubPattern对象)
        list *pubsub_patterns;
        ...
    }

    // 模式节点定义
    struct pubsubPattern 
    {
        client *client; // 订阅当前模式的客户端
        robj *pattern;  // 模式
    }

    struct client
    {
        ...
        // 客户端感兴趣的频道(相当于集合)
        // 键 => 频道名
        // 值 => NULL
        dict *pubsub_channels; 
        // 客户端感兴趣的模式存储在这里的链表(链表中只存储模式名)
        list *pubsub_patterns;
        ...
    }

## 二、名词解释
    无论是发布者还是订阅者相对redisServer都是客户端

    频道:字符串 字典pubsub_channels的键(key)
    订阅频道:将当前客户端加入服务器字典pubsub_channels中key(频道)对应的value中(客户端链表)
    退订频道:客户端请求在服务端pubsub_channels字典中key(频道)对应的value(客户端链表)中摘除自己

    模式:带正则表达式的字符串
    订阅模式:当前客户端和当前客户端关注的模式组成节点添加到服务端pubsub_patterns链表,多个客户端订阅同一个模式会有多个节点被加入
    退订模式:当前客户端发起请求在服务端pubsub_patterns链表中找到与自己模式匹配且客户端匹配的节点摘除
    
    发布:客户端向服务端某个频道推送消息(服务端收到发布指令后会将消息传播到所有订阅者),服务端在pubsub_channels找到对应key(频道)的value(客户端链表)依次推送消息,在pubsub_patterns链表中遍历节点依次模式匹配,匹配成功则推送消息

## 三、redis命令
    SUBSCRIBE 订阅频道
        subscribeCommand(client *c)
    UNSUBSCRIBE 退订频道
        unsubscribeCommand(client *c)
    PSUBSCRIBE 订阅模式
        psubscribeCommand(client *c)
    PUNSUBSCRIBE 退订模式
        punsubscribeCommand(client *c)
    PUBLISH 发布
        publishCommand(client *c)



# redis事务
    事务提供一种将命令打包然后一次性有序执行的机制
    多个命令会被入队到事务队列中 按照先进先出的顺序执行
    UNWATCH 取消 WATCH 命令对所有 key 的监视
    DISCARD 取消事务 放弃执行事务块内的所有命令

## 一、底层数据结构
    struct client
    {
        multiState mstate; // 事务状态 主要是事务队列
    }

    struct multiState 
    { 
        multiCmd *commands;     // 事务队列 fifo顺序
        int count;              // 已入队命令计数
        int minreplicas;            /* MINREPLICAS for synchronous replication */
        time_t minreplicas_timeout; /* MINREPLICAS timeout as unixtime. */
    }

    struct multiCmd 
    {
        robj **argv;                // 参数
        int argc;                   // 参数数量
        struct redisCommand *cmd;   // 命令指针
    }

    struct redisDb 
    {
        // 数据库中正在被watch(乐观锁)命令监视的键的字典
        // key 数据库键
        // value 监视该键的客户端链表
        dict *watched_keys;
    }

## 二、redis事务的完整执行流程
    WATCH key1 ... keyn     // 乐观锁锁住要修改的key
    MULTI   // 事务开始 multiCommand
                                | -> c->flags |= CLIENT_MULTI // 打开事务状态
    ...     // 需要执行的命令 processCommand命令入队 
                                    | -> queueMultiCommand // 客户端处在事务状态 且当前执行的不是EXEC DISCARD MULTI WATCH命令则入队
    EXEC    // 事务提交 execCommand
                            | -> if (c->flags & (CLIENT_DIRTY_CAS|CLIENT_DIRTY_EXEC)) { 取消事务 } // 不安全
                            | -> unwatchAllKeys // 取消客户端对所有键的监视
                            | -> loop c->mstate : call(c,CMD_CALL_FULL) // 循环执行事务队列中的命令
                            | -> discardTransaction // 取消事务并清除客户端标志 CLIENT_MULTI CLIENT_DIRTY_CAS CLIENT_DIRTY_EXEC

## 三、事务安全性的保证
    WATCH key1 ... keyn     // 乐观锁 在EXEC执行之前 监视任意数量的数据库键 在EXEC执行时 检查被监视的键是否至少有一个已经被修改过 若是则拒绝执行事务执行 确保事务安全性

    CLIENT_DIRTY_CAS客户端携带此标记表示事务已不安全

    任何修改数据库的命令SET LPUSH SADD ZREM DEL FLUSHDB...      // 对key执行修改操作
                | -> signalModifiedKey or signalFlushedDb       
                            | -> touchWatchedKey or touchWatchedKeysOnFlush   // 遍历watched_keys检查当前修改的key是否正在被监视
                                                    | -> c->flags |= CLIENT_DIRTY_CAS   // 若key正在被监视 则将对应的value(客户端链表依次)打上CLIENT_DIRTY_CAS标记

## 四、事务的特性
    Automicity  事务中的多个操作当作整体来执行 要么执行事务中的所有操作 要么一个操作也不执行
    Consistency 数据库执行之前是一致的 执行事务之后 无论成功与否 数据库应该依然是一致的
    Isolation   数据库有多个事务并发执行 各事务之间互不影响 并发状态下执行的事务和串行执行的事务结果相同
    Durability  事务执行完毕 结果被保存到永久性存储介质 即使服务器执行事务后停机 事务结果也不会丢失