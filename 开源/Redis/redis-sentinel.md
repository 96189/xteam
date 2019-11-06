# sentinel系统(redis高可用方案)
+    分布式系统一致性Raft算法
    
    http://thesecretlivesofdata.com/raft/
    https://raft.github.io/
    https://github.com/maemual/raft-zh_cn/blob/master/raft-zh_cn.md
    
    https://blog.csdn.net/gqtcgq/article/category/5992975

    图
    sentinel系统中的节点表示、监控分组、连接建立
    sentinel系统定时处理函数sentinelTimer
    void sentinelTimer(void) 
    {
        sentinelCheckTiltCondition();
        sentinelHandleDictOfRedisInstances(sentinel.masters);
        sentinelRunPendingScripts();
        sentinelCollectTerminatedScripts();
        sentinelKillTimedoutScripts();
        server.hz = CONFIG_DEFAULT_HZ + rand() % CONFIG_DEFAULT_HZ;
    }    
    sentinel系统的主函数sentinelHandleRedisInstance
    sentinel节点与master、slave节点的两条连接的作用(命令连接和订阅连接)
    sentinel节点之间如何互相发现
    定时发送命令的周期以及命令的用途及命令的回调处理
    判断实例是否主观下线
    选举流程及状态转移
        发起下线 -> 投票 -> 计票 -> 状态转移
    TILT模式
    sentinel执行脚本任务


    sentinel选举
    发布订阅模式
    主观下线
    客观下线
    slave选择规则与提升为master