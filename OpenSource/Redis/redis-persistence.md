# Redis持久化(persistence)
    Redis 的持久化运作方式 https://redis.io/topics/persistence
    http://oldblog.antirez.com/post/redis-persistence-demystified.html

## 一、RDB(Redis Database)
    时间点快照存储
### 1、save和bgsave
    rdbSave rdbSaveBackground
### 2、rdb载入与数据还原 
    rdbLoad
### 3、定时bgsave机制 
    serverCron

## 二、AOF(Append Only File)
    写操作记录日志

### 1、aof持久化功能的实现步骤(每一步执行时机)
    命令追加到aof_buf
        lookupKeyRead/Write -> expireIfNeeded -> propagateExpire -> feedAppendOnlyFile
    文件写入
        beforeSleep -> flushAppendOnlyFile
    文件同步
        根据配置策略
            AOF_FSYNC_ALWAYS    写入后立即同步到磁盘
            AOF_FSYNC_EVERYSEC  写入后每秒同步一次
            不配置               写入文件何时同步由操作系统决定

### 2、AOF文件载入与数据还原
    loadAppendOnlyFile

### 3、AOF文件重写BGREWRITEAOF
    rewriteAppendOnlyFileBackground 执行时机serverCron
    rewriteAppendOnlyFile
#### (1) 为什么要执行重写?
#### (2) 如何执行重写?

### 4、AOF文件重写子进程重写父进程依然处理命令,如何解决当前数据库和aof文件数据不一致的问题?
    在子进程进行aof文件重写的过程中,父进程依然处理命令,但feedAppendOnlyFile函数会将当前执行的命令存储在重写缓冲区aof_rewrite_buf_blocks

## 三、持久化方案的选择

## 四、在*nix操作系统,如何保证对文件的更新内容成功持久化到硬盘?
    linux同步i/o sync fsync fdatasync
