# 集群cluster
    http://www.redis.cn/topics/cluster-tutorial.html
    http://www.redis.cn/topics/cluster-spec.html
    https://blog.csdn.net/gqtcgq/article/category/5992975
    CAP理论
    数据分片-哈希槽
    主从复制模型
    一致性保证(可能丢失写操作->异步复制和网络分区)

    集群网络拓扑结构
    节点的自动发现(节点握手)
    Move重定向
    Ask重定向
    集群在线重配置
    失效检测
    节点选举
    服务器哈希槽信息的传播规则
    备份迁移

    clusterCron

    clusterInit -> aeCreateFileEvent(server.el, server.cfd[j], AE_READABLE,clusterAcceptHandler, NULL) -> aeCreateFileEvent(server.el,cfd,AE_READABLE,clusterReadHandler,link) -> clusterProcessPacket(命令处理函数)


## 集群节点间是如何认识的?
### 0、MEET命令
### 1、两个节点间是如何认识的?
### 2、一个节点和包含n个节点的集群是如何认识的?
### 3、分布式基础通信协议--gossip协议

## 心跳消息和下线检测 -> clusterCron
### 1、心跳消息
### 2、疑似下线(PFAIL)
### 3、下线(FAIL)
    
## 集群键的分配与迁移
### 1、对集群节点分配槽位
        CLUSTER  ADDSLOTS  <slot>  [slot]  ...
        clusterNodeSetSlotBit(myself,slot)  // 添加位图
        server.cluster->slots[slot] = myself // 添加slot-> clusterNode的映射关系
### 2、重新分片(槽位迁移)流程
####    (1)、通知迁入节点
        CLUSTER  SETSLOT  <slot>  IMPORTING  <node> // <slot>是要迁入的槽位号 <node>是当前负责该槽位的节点
        server.cluster->importing_slots_from[slot] = node
####    (2)、通知迁出节点
        CLUSTER  SETSLOT  <slot>  MIGRATING  <node> // <slot>是要迁出的槽位号 <node>是迁出槽位的目的地节点
        server.cluster->migrating_slots_to[slot] = node
####    (3)、向迁出节点查询slot上对应的key总数
        CLUSTER  GETKEYSINSLOT  <slot>  <count> // 获得迁出槽位<slot>中的<count>个key以便下一步能够执行key的迁移操作
####    (4)、命令迁出节点迁出数据
        迁出节点连接到迁入节点migrateGetSocket
        MIGRATE <target_host> <target_port> <key> <target_database> <timeout> // migrateCommand迁移键
        迁出节点循环构造RESTORE <key> <ttl> <serialized-value> [REPLACE]或者RESTORE-ASKING  <key>  <ttl>  
        <serialized-value> [REPLACE]
        发送循环构造好的命令到迁入节点
####    (5)、命令迁入节点存储数据
        restoreCommand处理命令
        迁入节点解析RESTORE <key> <ttl> <serialized-value> [REPLACE]或者RESTORE-ASKING  <key>  <ttl>  
        <serialized-value> [REPLACE]
        解析结果存储到数据库
####    (6)、周知所有集群节点发生了重新分片
        CLUSTER  SETSLOT  <slot>  NODE  <nodeid> // 通知所有节点 更新槽位<slot> 新的负责节点为<nodeid>
        当前正在迁入的节点收到本消息 server.cluster->importing_slots_from[slot] = NULL
        当前正在迁出的节点收到本消息且已经迁完 server.cluster->migrating_slots_to[slot] = NULL
        每一个收到本消息的节点 
        清除旧的位图并解除slot-> clusterNode的映射关系clusterNodeClearSlotBit(n,slot) server.cluster->slots[slot]= 
    NULL
        添加新的位图并添加slot-> clusterNode的映射关系clusterNodeSetSlotBit(n,slot) server.cluster->slots[slot]=n
### 3、连接池
####    (1)、缓存连接的时机
        migrateCommand -> migrateGetSocket
####    (2)、定期清理连接池 
        serverCron -> migrateCloseTimedoutSockets
## 集群节点执行命令
### 1、执行命令的过程
        processCommand -> getNodeByQuery(查询能集群中能处理当前命令的节点) -> 
### 2、ASK重定向
        查询集群节点名单命中当前节点
        对当前命令的key经过hash计算得出的slot槽虽然现在属于本节点处理 但本节点正在将keys迁移到其他节点
### 3、MOVED重定向
        查询集群节点名单命中其他节点
        对当前命令的key经过hash计算得出的slot槽不属于本节点处理 将请求重定向到其他节点 

## 集群的主从复制和故障转移
### 1、主从复制
        主从复制的先决条件:拒绝自我复制 拒绝复制从节点 当前节点已经是master节点且已经负责一部分的slot 当前节点已经是master
    节点且当前节点的数据库键不空拒绝执行复制
        集群中主从复制流程:当前集群节点和目标master集群节点状态维护 执行replicationSetMaster
### 2、故障转移
#### (1)、epoch概念
        集群状态结构体 clusterState { currentEpoch }
        集群消息结构体 clusterMsg { currentEpoch, configEpoch }
        currentEpoch是一个集群状态相关的概念,可以当做记录集群状态变更的递增版本号.当前节点接收到来自其他节点的包时,如果发送
    者的currentEpoch大于当前节点的currentEpoch,那么当前节点会更新currentEpoch为发送者的currentEpoch.因此,集群中所有节点的currentEpoch最终会达成一致(gossip协议),相当于对集群状态的认知达成了一致.

        configepoch是一个集群节点配置相关的概念,每个集群节点都有自己独一无二的configepoch.所谓的节点配置,实际上是指节点所负
    责的槽位信息(是关于cluster slot的一个版本号).configEpoch主要用于解决不同的节点的配置(slot)发生冲突的情况.
#### (2)、故障转移执行的时机
        clusterCron -> 若当前节点是slave节点则无条件执行 clusterHandleSlaveFailover(故障转移检测及处理)
#### (3)、故障转移流程
##### (a)、发现故障后延迟投票
        若某一个slvae节点发现自己的master节点被标记为下线(FAIL),计算延迟投票开始的时间,开始向master下属的所有slave节点发送
    PONG包,包头中携带当前节点的复制偏移,其他slave节点收到之后更新集群节点中sender的rank
##### (b)、请求给自己投票   
        由于clusterCron的定时调用,直到投票时间到达,发起投票请求clusterRequestFailoverAuth,复制数据最多的slave节点最先请求
    发起投票
##### (c)、集群master节点投票    
        集群节点收到投票请求后调用clusterSendFailoverAuthIfNeeded投票,只有负责槽位的master节点可以投票
    clusterSendFailoverAuth
##### (d)、发起投票的节点计票    
        发起投票的slave节点统计票数failover_auth_count++,然后标记在下一次事件循环开始之前执行验票操作
##### (e)、验票    
        执行验票操作clusterHandleSlaveFailover,票数达到要求则投票过程完成.
##### (f)、广播新master节点    
        赢得选举的slave节点取代master并广播到集群通知更新状态
##### (g)、更新配置    
        更新配置clusterProcessPacket,槽位slots负责权的更新,以及新master的节点和slave节点的从属关系更新
    clusterUpdateSlotsConfigWith

### 3、故障转移过程中的一些问题
#### (1)、为什么要延迟投票?如何防止多个节点同时发起投票?如何保证复制数据量最多的节点最先发起投票?
    mstime() + 500 + random() % 500 + server.cluster->failover_auth_rank * 1000
#### (2)、如何避免某一个slave节点在接受投票的过程中其他slave节点发起投票操作?
        clusterSendFailoverAuthIfNeeded如果当前节点lastVoteEpoch，与当前节点的currentEpoch相等，说明本界选举中，当前节点
    已经投过票了，不在重复投票，直接返回（因此，如果有两个从节点同时发起拉票，则当前节点先收到哪个节点的包，就只给那个节点投票。注意，即使这两个从节点分属不同主节点，也只能有一个从节点获得选票）
        针对同一个下线主节点，在2*server.cluster_node_timeout时间内，只会投一次票，这并非必须的限制条件（因为之前的
        lastVoteEpoch判断，已经可以避免两个从节点同时赢得本界选举了），但是这可以使得获胜从节点有时间将其成为新主节点的消息
    通知给其他从节点，从而避免另一个从节点发起新一轮选举又进行一次没必要的故障转移
