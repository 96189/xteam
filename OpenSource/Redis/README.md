
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
    hash函数生成算法 MurmurHash2
    hash冲突解决:链地址法
    hash表的扩展与收缩
    redis的渐进式rehash算法(重新散列算法)

## 字典
    redis中字段的使用哈希表实现

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
    
# 0x02 redis对象
    redis对象系统使用void decrRefCount(robj *o)接口实现了基于引用计数的内存回收机制,使用void incrRefCount(robj *o)接口通过引用计数实现了对象共享机制.
    对外提供统一的对象接口
    typedef struct redisObject {
        unsigned type:4;
        unsigned encoding:4;
        unsigned lru:LRU_BITS; 
        int refcount;
        void *ptr;
    } robj;

## 字符串对象
### 底层数据结构
    sds
    可存储整数字符串 浮点型字符串 普通字符串
    
## 列表对象
### 底层数据结构
    quicklist
    
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


# 0x03 缓存淘汰算法(内存释放)
    freeMmoryIfNeeded() //每当执行一个命令的时候，就会调用该函数来检测内存是否够用
    https://blog.csdn.net/wuliusir/article/details/51598226
    https://blog.csdn.net/qq_35440678/article/details/53453107
## LRU

## LFU


# Scan命令

# 0x04 Redis过期键(expire key)的删除策略
## 惰性删除策略(expireIfNeeded)
    执行时机
## 定期删除策略(activeExpireCycle)
    执行时机

# 0x05 Redis持久化(persistence)
    Redis 的持久化运作方式 https://redis.io/topics/persistence
    http://oldblog.antirez.com/post/redis-persistence-demystified.html

## redis持久化(存储)策略
### RDB(Redis Database)
    时间点快照存储
#### save和bgsave
    rdbSave rdbSaveBackground
#### 数据库启动载入rdb文件 
    rdbLoad
#### 定时bgsave机制 
    serverCron

### AOF(Append Only File)
    写操作记录日志

#### aof持久化功能的实现步骤
    命令追加到aof_buf
        lookupKeyRead/Write     ->  expireIfNeeded      ->  propagateExpire     ->  feedAppendOnlyFile
    文件写入
        beforeSleep  ->  flushAppendOnlyFile
    文件同步
        根据配置策略
            AOF_FSYNC_ALWAYS    写入后立即同步到磁盘
            AOF_FSYNC_EVERYSEC  写入后每秒同步一次
            不配置               写入文件何时同步由操作系统决定

#### 在*nix操作系统,如何保证对文件的更新内容成功持久化到硬盘?
    linux同步i/o sync fsync fdatasync

#### AOF文件载入与数据还原
    loadAppendOnlyFile

#### AOF文件重写BGREWRITEAOF
    rewriteAppendOnlyFileBackground
    rewriteAppendOnlyFile

#### AOF文件重写子进程重写父进程依然处理命令,如何解决当前数据库和aof文件数据不一致的问题?
    在子进程进行aof文件重写的过程中,父进程依然处理命令,但feedAppendOnlyFile函数会将当前执行的命令存储在重写缓冲区aof_rewrite_buf_blocks

# XXX
## 时间事件的应用实例serverCron
### 更新服务器的各类统计信息
### 清理数据库中过期的键值对
### 关闭和清理连接失效的客户端
### 尝试进行AOF或RDB持久化操作
### 如果服务器是主服务器,则对从服务器进行定期同步
### 如果处于集群模式,对集群进行定期同步和连接测试

## 事件调度和执行规则

# 全局唯一id生成方法
    事件定时器id和客户端id

## 创建客户端并挂载到server的clients链表尾部的时机
    acceptTcpHandler -> acceptCommonHandler -> createClient -> listAddNodeTail
## redis命令的执行
    readQueryFromClient -> processInputBuffer -> processCommand -> call
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
    
    