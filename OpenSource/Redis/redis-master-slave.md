# 主从同步
    官方文档 https://redis.io/topics/replication
## 命令
    SYNC                slave节点请求同步
    PSYNC               slave节点请求部分同步
    SLAVEOF ip port     将当前服务器转变为指定(ip,port)服务器的slave节点
    SLAVEOF NO ONE      将当前slave节点提升为master节点,关闭复制功能,原来同步的数据集不会丢弃
## 一、主从复制流程 
    1、slave与master握手
    2、master向slave传输文件
    3、master向slave传播命令
## 二、主从复制的相关问题:
    1、文件传输中断后重连如何处理?
    2、命令传播中断后重连如何处理?
        slave再次发起增量同步,将slave记录的偏移量传递给master,master根据此值在积压缓冲区中的特定位置开始传输数据.

    3、命令传播的实现机制?如何保证命令在主从都会执行(事务)?
        master异步传播命令到slave,不保证slave一定执行。即使slave没有收到或者收到后没有执行,短时间内可能会不一致,但是由于定时机制,slave上报自己的复制偏移量,会再执行增量同步.这体现了redis异步复制的最终一致性.
    
    4、slave状态转换


## 三、如何保证数据一致性
    主从复制是异步复制 弱一致性 最终一致性 短时间内是有可能不一致 但最终会达到一致
    全量同步(或增量同步)+命令传播
    先尝试 增量同步 服务端不允许增量同步 则全量同步
### 数据同步
#### 全量同步
    rdb文件传输
    slave角度:
        syncWithMaster // slave->master连接上的读写事件处理函数
                    | -> syncWrite("SYNC")
    master角度:
        syncCommand
            | -> startBgsaveForReplication
    
    slave回调(接收master传输的rdb文件)
        readSyncBulkPayload

#### 增量同步
    积压缓冲区内容传输
    slave角度:
        syncWithMaster // slave->master连接上的读写事件处理函数
                    | -> slaveTryPartialResynchronization
                            | -> sendSynchronousCommand("PSYNC", psync_replid, psync_offset)
    master角度:
        syncCommand
            | -> masterTryPartialResynchronization
                            | -> addReplyReplicationBacklog // 发送server.repl_backlog积压缓冲区

    slave回调(master传输积压缓冲区中的命令 master相当于slave的客户端)
        readQueryFromClient

#### 积压空间的空间分配及回收以及内容收发
    master的积压空间repl_backlog是所有slave共享的
##### 积压缓冲区空间申请 createReplicationBacklog
    master的syncCommand函数中调用
    ? slave为什么需要积压缓冲区 ? 
    slave的readSyncBulkPayload和slaveTryPartialResynchronization函数中调用

##### 积压缓冲区空间释放 freeReplicationBacklog
    slave的syncWithMaster
    master的replicationCron

##### 填充数据到积压缓冲区 feedReplicationBacklog
    propagate
        | -> replicationFeedSlaves // 命令写入积压缓冲区 传播命令到slave节点
    propagateExpire
        | -> replicationFeedSlaves // 命令写入积压缓冲区 传播命令到slave节点
##### 积压缓冲区数据读取
    addReplyReplicationBacklog

### 命令传播
    客户端发给master的命令 都将被传播到slave节点 异步发送且master并不关心slave的执行结果 slave判断是master发来的命令处理后也不会有返回

#### 写命令传播
    call
      | -> propagate
                  | -> replicationFeedSlaves

#### 键过期传播
    expireIfNeeded
        | -> propagateExpire
                    | -> replicationFeedSlaves

### 定时任务保证命令传播的正确性
    replicationCron
                 | -> replicationSendAck() // slave定期向主服务器发送 ACK 命令 和 同步进度reploff
                 | -> replicationFeedSlaves("PING") // master定时向它们发送 PING 检测slave的存活

    
    
参考 
    https://blog.csdn.net/gqtcgq/article/details/51172085
    http://wiki.jikexueyuan.com/project/redis/master-slave-replication.html
    http://ningg.top/redis-lesson-8-redis-master-slave/
    https://jiachuhuang.github.io/2017/07/17/Redis%E4%B8%BB%E4%BB%8E%E5%A4%8D%E5%88%B6%E2%80%94%E2%80%94Slave%E8%A7%86%E8%A7%92/
