
##  CAP理论
+   Consistency一致性
    
    所有的节点数据一致
+   Availability可用性

    一个或者多个节点失效,不影响服务请求
+   PartitionTolerance分区容错性

    节点间的网络连接失效,仍然可以处理请求

+   CAP在实践中的取舍

##  BASE理论
+   BasicallyAvailable(基本可用)
+   Soft-state(软状态)
+   EventuallyConsistent(数据的最终一致性)

##  分布式系统的特性
+   高可用
    *   避免单点故障
        *   冗余
        *   自动故障转移
    *   故障监控自动预警
    *   服务降级、限流自我保护
    *   数据量(数据分片、读写分离)
+   一致性
    *   强一致性
    *   弱一致性
    *   最终一致性
+   高性能
+   可扩展性

## 分布式系统面临的主要问题
+   分布式系统如何维护多个节点之间的一致性

    *   共识算法
        *   Paxos
        *   Raft

    *   一致性协议
        *   gossip
        *   zab

## 分布式系统中具体的细节
+   [常见负载均衡算法及应用场景](https://github.com/96189/xteam/blob/master/%E5%BC%80%E6%BA%90/Nginx/src/ngx_load_balance.md)

## 分布式系统实践
+   redis的分布式实践
+   tars的分布式实践
+   mysql的分布式实践
+   游戏服务的分布式实践
    *   实现高可用消除单点故障

        系统功能尽可能拆分,一个服务提供一个功能,尽量使每个服务做到无状态,讲状态存储在缓存中,若有必要可将缓存中的状态数据落地.每个服务通过操作缓存来进行状态的修改.每个服务多进程部署,服务间的调用进行负载均衡.对每个服务做监控以及自动拉起功能.

    *   游戏中常见的功能模块

        登录认证
        
        游戏匹配与房间状态(总数、当前在用、每个游戏在线在玩)

        数据统计与上报

        机器人系统
    
    *   游戏系统设计

        接入层消息分号段方便扩展,提前预定号段划分功能,避免在接入层硬编码导致频繁改动

        游戏重连要在匹配逻辑实现,匹配前优先检查是否在游戏中,如果在则强制拉回原来的游戏中(判断玩家是否在玩)

        游戏逻辑与匹配逻辑的房间信息状态同步,定时状态检查与定时状态统计

        退休机制


## 分布式系统的常用组件
+   消息中间件
    *   [zmq](https://github.com/96189/xteam/tree/master/%E5%BC%80%E6%BA%90/libzmq/my-dir)

+   服务注册与发现


##  微服务

## 相关文章
+   [什么是分布式系统,如何学习分布式系统](https://www.cnblogs.com/xybaby/p/7787034.html)
+   [分布式系统核心问题](https://blog.csdn.net/zyhlwzy/article/details/78658002)
+   [分布式系统高可用设计](https://blog.csdn.net/zxp_cpinfo/article/details/53869654)
+   [分布式系统设计](https://blog.csdn.net/zhengchao1991/article/details/81071725)
+   [分布式系统设计系列 -- 基本原理及高可用策略](https://www.cnblogs.com/zhengzhamn/articles/5322023.html)