# 主从同步
    https://redis.io/topics/replication
    CMD
        SYNC
        PSYNC
        SLAVEOF ip port
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