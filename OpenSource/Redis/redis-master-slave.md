# 主从同步
    官方文档 https://redis.io/topics/replication
    参考 https://blog.csdn.net/gqtcgq/article/details/51172085
## 命令
    SYNC                slave节点请求同步
    PSYNC               slave节点请求部分同步
    SLAVEOF ip port     将当前服务器转变为指定(ip,port)服务器的slave节点
    SLAVEOF NO ONE      将当前slave节点提升为master节点,关闭复制功能,原来同步的数据集不会丢弃
## 一、主从复制流程 
    replicationCron() -> connectWithMaster()
                        replicationSendAck()
                        replicationFeedSlaves("PING")
                        ...
    connectWithMaster() -> syncWithMaster() slave与master连接建立事件处理器
                                            readSyncBulkPayload() 文件传输处理器

## 二、如何保证数据一致性
    全同步
    部分同步

    http://wiki.jikexueyuan.com/project/redis/master-slave-replication.html
    http://ningg.top/redis-lesson-8-redis-master-slave/