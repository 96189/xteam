
# 0x01 底层数据结构
## sds字符串对象
    sds对象设计的几个优点
        1、常数复杂度获取长度
        2、杜绝缓冲区溢出
        3、减少修改字符串长度时所需的内存重新分配次数
        4、二进制安全
        5、兼容部分c函数

## 双向链表
    双向链表常见的操作实现
    
## Quicklist
    ziplist和双向链表的结合(列表对象实际使用的数据结构)
    
## 跳跃表
    [skiplist论文](ftp://ftp.cs.umd.edu/pub/skipLists/skiplists.pdf)
    平均O(logN)复杂度的证明 
    幂次定律随机生成层高
    redis跳跃表的几个特点(score可相同,后退指针)

## 哈希表
    hash函数生成算法 siphash https://en.wikipedia.org/wiki/SipHash
    hash冲突解决:链地址法
    hash表的扩展与收缩
    redis的渐进式rehash算法(重新散列算法)

## 字典
    哈希表实现字典

## 整数集合

## ziplist压缩列表
    空白 ziplist 示例图
    area        |<---- ziplist header ---->|<-- end -->|
    size          4 bytes   4 bytes 2 bytes  1 byte
                +---------+--------+-------+-----------+
    component   | zlbytes | zltail | zllen | zlend     |
                |         |        |       |           |
    value       |  1011   |  1010  |   0   | 1111 1111 |
                +---------+--------+-------+-----------+
                                           ^
                                           |
                                   ZIPLIST_ENTRY_HEAD
                                           &
    address                        ZIPLIST_ENTRY_TAIL
                                          &
                                  ZIPLIST_ENTRY_END
    非空 ziplist 示例图
    area        |<---- ziplist header ---->|<----------- entries ------------->|<-end->|
    size          4 bytes  4 bytes  2 bytes    ?        ?        ?        ?     1 byte
                +---------+--------+-------+--------+--------+--------+--------+-------+
    component   | zlbytes | zltail | zllen | entry1 | entry2 |  ...   | entryN | zlend |
                +---------+--------+-------+--------+--------+--------+--------+-------+
                                           ^                          ^        ^
    address                                |                          |        |
                                   ZIPLIST_ENTRY_HEAD                 |   ZIPLIST_ENTRY_END
                                                                      |
                                                            ZIPLIST_ENTRY_TAIL
                                                        
    Ziplist 是为了尽可能地节约内存而设计的特殊编码双端链表,Ziplist 允许在列表的两端进行 O(1) 复杂度的 push 和 pop 操作。
    已知节点指针p,p-p->prevlensize得到前一个节点的地址,p+p所占的字节数得到后一个节点的地址.


    ziplist如何做到节省内存？
        ziplist的内存是连续的
        entry-n的内存布局:
            1 or 5字节编码前置节点的长度 | 1 or 2 or 5字节编码当前节点值的长度 | 值

    
# 0x02 redis对象
    redis对象系统使用void decrRefCount(robj *o)接口实现了基于引用计数的内存回收机制,使用void incrRefCount(robj *o)接口通过引用计数实现了对象共享机制.
    对外提供统一的对象接口
    typedef struct redisObject {
        unsigned type:4;
        unsigned encoding:4;
        unsigned lru:LRU_BITS; 
        int refcount;
        void *ptr; // 具体的底层数据结构
    } robj;

## 字符串对象
### 底层数据结构
    sds
    可存储整数字符串 浮点型字符串 普通字符串
    
## 列表对象
### 底层数据结构
    quicklist = list + ziplist
    
## 集合对象
### 底层数据结构
    hash table
    intset
    
## 哈希对象
### 底层数据结构
    hash table
    ziplist
    
## 有序集合对象
### 底层数据结构
    hash table+skiplist
    ziplist
    
## Module对象
### 底层数据结构


# 0x03 缓存淘汰策略(内存释放)
    https://redis.io/topics/lru-cache
  
    freeMemoryIfNeeded()      // 命令执行时,检查内存是否够用

## LRU/LFU结合allkeys和volatile数据集淘汰缓存
    LFU详细 http://antirez.com/news/109
### LRU/LFU实现原理
    typedef struct redisObject {
        unsigned type:4;
        unsigned encoding:4;
        unsigned lru:LRU_BITS; /* LRU time (relative to global lru_clock) or
                                * LFU data (least significant 8 bits frequency
                                * and most significant 16 bits access time). */
        int refcount;
        void *ptr;
    } robj;

### 初始化
    createObject(...)
        o->lru = LRU_CLOCK()
        o->lru = (LFUGetTimeInMinutes()<<8) | LFU_INIT_VAL

### 更新
    lookupKey(...)
        val->lru = LRU_CLOCK()
        updateLFU(val)

### LRU/LFU算法实现
    evictionPoolPopulate(...) // 随机采样存储数据到pool
        LRU
            pool中存储server的lru与样本robj的lru的时间差 升序排列
        LFU
            pool中存储(255-robj的频率计数器) 升序排列 保证使用频率最小的对象存储在最后以便淘汰

### redis中LRU和LFU的应用
    (LRU, LFU) * (allkeys, volatile) 共4中情况
    allkeys-lru     db->dict
    allkeys-lfu     db->dict
    volatile-lru    db->expires
    volatile-lfu    db->expires

## volatile-ttl淘汰缓存

## allkeys-random和volatile-random淘汰缓存

# Scan命令

# 0x04 Redis过期键(expire key)的删除策略
## 惰性删除策略(expireIfNeeded)
    执行时机
        任何对key的读写操作

## 定期删除策略(activeExpireCycle)
    执行时机    
        serverCron -> databasesCron -> activeExpireCycle
        beforeSleep -> activeExpireCycle

# 0x05 Redis持久化(persistence)
    Redis 的持久化运作方式 https://redis.io/topics/persistence
    http://oldblog.antirez.com/post/redis-persistence-demystified.html

## RDB(Redis Database)
    时间点快照存储
### save和bgsave
    rdbSave rdbSaveBackground
### rdb载入与数据还原 
    rdbLoad
### 定时bgsave机制 
    serverCron

## AOF(Append Only File)
    写操作记录日志

### aof持久化功能的实现步骤(每一步执行时机)
    命令追加到aof_buf
        lookupKeyRead/Write -> expireIfNeeded -> propagateExpire -> feedAppendOnlyFile
    文件写入
        beforeSleep -> flushAppendOnlyFile
    文件同步
        根据配置策略
            AOF_FSYNC_ALWAYS    写入后立即同步到磁盘
            AOF_FSYNC_EVERYSEC  写入后每秒同步一次
            不配置               写入文件何时同步由操作系统决定

### 在*nix操作系统,如何保证对文件的更新内容成功持久化到硬盘?
    linux同步i/o sync fsync fdatasync

### AOF文件载入与数据还原
    loadAppendOnlyFile

### AOF文件重写BGREWRITEAOF
    rewriteAppendOnlyFileBackground 执行时机serverCron
    rewriteAppendOnlyFile

### AOF文件重写子进程重写父进程依然处理命令,如何解决当前数据库和aof文件数据不一致的问题?
    在子进程进行aof文件重写的过程中,父进程依然处理命令,但feedAppendOnlyFile函数会将当前执行的命令存储在重写缓冲区aof_rewrite_buf_blocks

# 0x06 定时serverCron
## 时间事件的应用实例serverCron
### 更新服务器的各类统计信息
### 清理数据库中过期的键值对
### 关闭和清理连接失效的客户端
### 尝试进行AOF或RDB持久化操作
### 如果服务器是主服务器,则对从服务器进行定期同步
### 如果处于集群模式,对集群进行定期同步和连接测试

## serverCron函数执行都做什么?
    时间缓存避免每次调用执行系统时间函数
    瞬时指标抽样测算 命令执行次数 网络流入流出流量
    服务器lruclock缓存
    更新内存峰值缓存
    SIGTERM信号处理 安全关闭
    数据库db中键情况上报
    连接情况上报
    客户端资源定时管理 断开超时连接 缩小客户端输入缓冲
    数据库资源定时管理 主动删除过期键 尝试收缩字典
    执行被延迟的aof重写
    BGSAVE或者BGREWRITEAOF的善后工作
    检查是否需要执行bgsave和bgrewriteaof
    ...
    ...
# 0x07 网络库服务器客户端
## 事件调度和执行规则

## 创建客户端并挂载到server的clients链表尾部的时机
    acceptTcpHandler -> acceptCommonHandler -> createClient -> listAddNodeTail
## redis命令的执行
    readQueryFromClient -> processInputBuffer -> processCommand -> call

# 0x08 主从同步
    https://redis.io/topics/replication
    CMD
        SYNC
        PSYNC
        SLAVEOF ip port
    主从复制流程 
        replicationCron() -> connectWithMaster()
                             replicationSendAck()
                             replicationFeedSlaves("PING")
                             ...
        connectWithMaster() -> syncWithMaster() slave与master连接建立事件处理器
                                                readSyncBulkPayload() 文件传输处理器

    如何保证数据一致性
        全同步
        部分同步

    http://wiki.jikexueyuan.com/project/redis/master-slave-replication.html
    http://ningg.top/redis-lesson-8-redis-master-slave/
    
# 0x09 sentinel系统
    sentinel选举
    发布订阅模式
    主观下线
    客观下线
    slave选择与提升为master

# 0x0A 集群


# 0x0B 订阅与发布
    频道订阅 数据结构:服务端与客户端各自维护一份频道字典 
        服务端保存(channel_name,list_client)键值对 客户端保存(channel_name,NULL)键值对
    模式订阅 数据结构:服务端与客户端各自维护一个模式链表 
        服务端链表节点为[channel_pattern_name,client] 客户端链表节点为[channel_pattern_name]
    发布
        服务端查询自己的数据结构 频道相同或模式匹配后 发送到对应的客户端

# 0x0C redis事务
    事务提供一种将命令打包然后一次性有序执行的机制
## redis实现事务的CMD
    watch key1 key2 ...
    unwatch
    discard

    multi
    ...
    exec
## redis实现事务的数据结构
    client
        multiState mstate   // 事务队列
        list *watched_keys  // key list
    server
        redisDb
            dict *watched_keys  // key->client_list
## 触发监视机制watch的时机
    signalModifiedKey -> touchWatchedKey ->         c->flags |= CLIENT_DIRTY_CAS
    signalFlushedDb -> touchWatchedKeysOnFlush ->   c->flags |= CLIENT_DIRTY_CAS
    processCommand -> flagTransaction ->            c->flags |= CLIENT_DIRTY_EXEC

## redis事务
    Automicity
    Consistency
    Isolation
    Durability

# 0x0D redis bitops的相关应用
    512M = 512 * 1024KB * 1024Byte * 8bit = 4294967296 每一个位可以表示两种状态
    活跃人数统计(每日上线人数统计\最近n天活跃人数统计)
    判断当天是否是第一次登录(每天限领一次的逻辑判断)
    连续登录奖励发放的条件判断

# 0x0E HyperLogLog
    http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf
    http://algo.inria.fr/flajolet/Publications/DuFl03-LNCS.pdf

# 0x0n 全局唯一id生成方法
    事件定时器id
    客户端id
    服务id
    慢查询标识符

    HyperLogLog algorithm -> MurmurHash64A