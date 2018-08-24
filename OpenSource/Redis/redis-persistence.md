# Redis持久化(persistence)
    Redis 的持久化运作方式 https://redis.io/topics/persistence
    http://oldblog.antirez.com/post/redis-persistence-demystified.html

## 一、RDB(Redis Database)
    时间点快照存储
    
    rdb文件结构
    | REDIS | metedata | databases | EOF | check_sum |
    od命令分析rdb文件 redis-check_dump分析rdb文件

### 1、save和bgsave
    save命令 <-> rdbSave函数    // 执行备份会阻塞当前服务器进程,无法处理命令.
                    | -> rdbSaveRio     // 按照rdb文件结构将数据写入文件,其中最重要的部分是databases,会遍历所有数据库将值(k,v,expire)写入文件

    bgsave命令 <-> rdbSaveBackground函数     // 不阻塞服务器进程,执行备份(子进程)同时仍可处理命令(父进程).
                            | -> rdbSaveBackground
                                        | -> fork
                                        | -> 子进程 执行rdbSave(可能阻塞) 
                                                | 完成后写管道通知父进程(信号) 退出
                                        | -> 父进程 从调用处出返回 继续执行事件循环 处理命令 

    bgsave命令 子进程完成rdb文件持久化后 父进程等待子进程结束 接收子进程退出前发送的信号
        int serverCron(...)
        {
            ...
            if ((pid = wait3(&statloc,WNOHANG,NULL)) != 0) // 非阻塞等待子进程结束 并获取结束状态
            {
                ... 
                else if (pid == server.rdb_child_pid) 
                {
                    backgroundSaveDoneHandler(exitcode,bysignal); // 资源清理 状态重置
                    if (!bysignal && exitcode == 0) receiveChildInfo();
                }
                ...
            }
            ... 
        }

### 2、定时bgsave机制 
    serverCron 100ms执行一次
    定时执行条件 n秒内已执行m次修改操作 n m可配置 若未配置 使用默认值
    int serverCron(...)
    {
        for (j = 0; j < server.saveparamslen; j++) 
        {
            // n秒内已执行m次修改操作
            if (server.dirty >= sp->changes &&
                server.unixtime-server.lastsave > sp->seconds && ...)
            {
                rdbSaveBackground(server.rdb_filename,rsiptr);
            }
        }
    }

### 3、rdb载入与数据还原 
    rdbLoad
        | -> rdbLoadRio     // 按照rdb文件结构 检验magic 校验version 读取(k,v)并添加到数据库 设置expire 读取EOF 读取check_sum并校验



## 二、AOF(Append Only File)
    写操作记录日志

### 1、aof持久化功能的实现步骤(每一步执行时机)
#### 命令追加到aof_buf(每一个写命令执行后会被追加到aof_buf缓冲区)
        lookupKeyRead/Write -> expireIfNeeded -> propagateExpire -> feedAppendOnlyFile

#### 文件写入(每次事件分派之前执行,aof_buf内存缓冲区存aof文件)
        beforeSleep -> flushAppendOnlyFile

#### 文件同步(根据appendfsync选项决定文件数据何时刷新到磁盘)
        根据配置策略
            always    写入后立即同步到磁盘
            everysec  写入后每秒同步一次(不配置的默认值)
            no        写入文件何时同步由操作系统决定

### 2、AOF文件重写BGREWRITEAOF命令
    重写:实际是创建新文件替换旧文件,新文件通过遍历数据库键空间,手动构造写命令,然后将命令写入新文件.通过这种方式去除对某个key的多个冗余命令.

#### (1) 为什么要执行重写?
    aof文件随时间体积越来越大且aof文件中冗余命令过多,对某个key可能保存多达n个写操作(aof文件存储某个key多个时间点的状态)

#### (2) 如何执行重写?
    rewriteAppendOnlyFileBackground // 执行时机serverCron
                    | -> fork
                    | -> 子进程 rewriteAppendOnlyFile
                                        | -> rewriteAppendOnlyFileRio // 遍历键空间,手动构造每个key的写命令 并将命令写入文件
                                                        | -> aofReadDiffFromParent // 从管道中读出命令 写入文件
                                        | -> aofReadDiffFromParent // 从管道中读出命令 写入文件
                                        | 发送重写成功信号 子进程退出
                    | -> 父进程 从调用处出返回 继续执行事件循环 处理命令 将新命令都缓存到aof重写缓冲区aof_rewrite_buf_blocks

### 3、AOF文件重写子进程重写父进程依然处理命令,如何解决当前数据库和aof文件数据不一致的问题?
    在子进程进行aof文件重写的过程中,父进程依然处理命令,但feedAppendOnlyFile函数会将当前执行的命令存储在重写缓冲区aof_rewrite_buf_blocks

#### (1)、fork模型及进程间通信
    父子进程通信管道定义
    redisServer
    {
        ... 
        // AOF pipes used to communicate between parent and child during rewrite
        // 以下只用于aof rewrite期间
        // parent -> children data
        int aof_pipe_write_data_to_child;   // fd[1] 可写事件处理函数 aofRewriteBufferAppend 父进程写命令
        int aof_pipe_read_data_from_parent; // fd[0] 子进程读命令

        // children -> parent ack
        int aof_pipe_write_ack_to_parent;   // fd[3]
        int aof_pipe_read_ack_from_child;   // fd[2] 可读事件处理函数 aofChildPipeReadable
        
        // parent -> children ack
        int aof_pipe_write_ack_to_child;    // fd[5]
        int aof_pipe_read_ack_from_parent;  // fd[4]
        ...
        // Pipe used to write the child_info_data. 
        // 父子进程间消息传递 bgsave也用到这里
        int child_info_pipe[2];         
    }
    
    aofCreatePipes调用pipe3次关联描述符和aof rewrite变量
    openChildInfoPipe调用pipe1次打开父子进程消息传递通道child_info_pipe

#### (2)、新的命令怎么添加到重写缓冲区以及重写缓冲区内容如何传递给子进程?
    feedAppendOnlyFile
            | -> aofRewriteBufferAppend  // 命令追加到重写缓冲区aof_rewrite_buf_blocks
                            | -> aofChildWriteDiffData // 父进程将命令写入aof_pipe_write_data_to_child管道端
    
    子进程调用aofReadDiffFromParent从aof_pipe_read_data_from_parent管道端读出数据存储到aof_child_diff
    子进程调用rewriteAppendOnlyFile写aof_child_diff到文件中

#### (3)、父进程等待子进程退出
    serverCron
        | -> backgroundRewriteDoneHandler // 父进程等待子进程结束成功后调用 读出信号
                            | -> aofRewriteBufferWrite // 将aof_rewrite_buf_blocks中所有内容写入临时文件

### 4、AOF文件载入与数据还原
    loadDataFromDisk
            | -> loadAppendOnlyFile
                        | -> createFakeClient // 创建伪客户端
                        | -> cmd->proc(fakeClient) // 循环逐条重放命令

## 三、持久化方案的选择

## 四、在*nix操作系统,如何保证对文件的更新内容成功持久化到硬盘?
    linux同步i/o sync fsync fdatasync
