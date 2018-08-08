# 订阅与发布
## 一、频道订阅 数据结构:服务端与客户端各自维护一份频道字典 
    服务端保存(channel_name,list_client)键值对 客户端保存(channel_name,NULL)键值对
## 二、模式订阅 数据结构:服务端与客户端各自维护一个模式链表 
    服务端链表节点为[channel_pattern_name,client] 客户端链表节点为[channel_pattern_name]
## 三、发布
    服务端查询自己的数据结构 频道相同或模式匹配后 发送到对应的客户端

# redis事务
    事务提供一种将命令打包然后一次性有序执行的机制
## 一、redis实现事务的CMD
    watch key1 key2 ...
    unwatch
    discard

    multi
    ...
    exec
## 二、redis实现事务的数据结构
    client
        multiState mstate   // 事务队列
        list *watched_keys  // key list
    server
        redisDb
            dict *watched_keys  // key->client_list
## 三、触发监视机制watch的时机
    signalModifiedKey -> touchWatchedKey ->         c->flags |= CLIENT_DIRTY_CAS
    signalFlushedDb -> touchWatchedKeysOnFlush ->   c->flags |= CLIENT_DIRTY_CAS
    processCommand -> flagTransaction ->            c->flags |= CLIENT_DIRTY_EXEC

## 四、事务的特性
    Automicity
    Consistency
    Isolation
    Durability