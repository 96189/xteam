# 缓存淘汰策略(内存释放)
    https://redis.io/topics/lru-cache

## 一、执行流程
    int freeMemoryIfNeeded(void) 
    {
        ...
        while (mem_freed < mem_tofree)
        {
            // 处理5种淘汰策略
            // lru -> (allkeys-lru volatile-lru)
            // lfu -> (allkeys-lfu volatile-lfu)
            // volatile-ttl 在设置了过期时间的键空间中 具有更早过期时间的key优先移除
            if (server.maxmemory_policy & (MAXMEMORY_FLAG_LRU|MAXMEMORY_FLAG_LFU) ||
                server.maxmemory_policy == MAXMEMORY_VOLATILE_TTL)
            {
                while(bestkey == NULL)
                {
                    for (i = 0; i < server.dbnum; i++) // 遍历0-15号数据库
                    {
                        // allkeys数据集dict 或 volatile数据集expires
                        dst_dict = (server.maxmemory_policy & MAXMEMORY_FLAG_ALLKEYS) ? db->dict : db->expires
                        // 随机采样 
                        evictionPoolPopulate(i, dst_dict, db->dict, pool)
                    }
                    // 遍历样本集
                    for (...)
                    {
                        ... 
                        bestkey = dictGetKey(de)
                        break   // goto delete-free-flag
                        ...
                    }
                }
            }
            // 处理2种淘汰策略
            // allkeys-random 在主键空间中 随机移除某个key
            // volatile-random 在设置了过期时间的键空间中 随机移除某个key
            else if (server.maxmemory_policy == MAXMEMORY_ALLKEYS_RANDOM ||
                     server.maxmemory_policy == MAXMEMORY_VOLATILE_RANDOM)
            {
                for (i = 0; i < server.dbnum; i++)  // 遍历0-15号数据库
                {
                    // allkeys数据集dict 或 volatile数据集expires
                    dst_dict = (server.maxmemory_policy == MAXMEMORY_ALLKEYS_RANDOM) ? db->dict : db->expires
                    de = dictGetRandomKey(dict)  // 随机获取某个key
                    bestkey = dictGetKey(de)    
                    break   // goto delete-free-flag
                }
            }
            // delete-free-flag
            if (bestkey) 
            {
                robj *keyobj = createStringObject(bestkey,sdslen(bestkey))
                dbSyncDelete(db,keyobj) // 执行删除
                mem_freed += 释放的内存
            }
        }
    }

## 二、cache置换算法介绍
    https://stackoverflow.com/questions/17759560/what-is-the-difference-between-lru-and-lfu
    LeastRecentlyUsed     LRU 最近最少使用
        如果一个数据在最近一段时间没有被访问到，那么在将来它被访问的可能性也很小
    LeastFrequentlyUsed   LFU 最低频率使用
        如果一个数据在最近一段时间内使用次数很少 那么在将来一段时间内被使用的可能性也很小

    LFU和LRU的区别
        考虑一个缓存容量为3的恒定缓存请求流
        A, B, C, A, A, A, A, A, A, A, A, A, A, A, B, C, D
        使用LRU算法
        [A]
        [A, B]
        [A, B, C]
        [B, C, A] <- a stream of As keeps A at the head of the list.
        [C, A, B]
        [A, B, C]
        [B, C, D] <- here, we evict A, we can do better!     
        使用LFU算法
        A - 12
        B - 2
        C - 2
        D - 1

    LFU和LRU的实现
        https://github.com/96189/xteam/blob/master/NetworkProgram/cache.h

## 三、执行时机
    processCommand
        | -> if (server.maxmemory)
             {
                freeMemoryIfNeeded()      // 命令执行时 检查内存是否够用
             }

## 四、LRU/LFU结合allkeys和volatile数据集淘汰缓存
    LFU详细 http://antirez.com/news/109
### 1、LRU/LFU实现原理
    struct redisObject 
    {
        unsigned type:4;
        unsigned encoding:4;
        unsigned lru:LRU_BITS; // LRU time (relative to global lru_clock) or LFU data (least significant 8 bits frequency and most significant 16 bits access time). 
        int refcount;
        void *ptr;
    }
    若配置LFU算法,则lru变量低8位为访问频率计数器,高16位为访问时间
### 2、初始化
    createObject(...)
        if (server.maxmemory_policy & MAXMEMORY_FLAG_LFU)
            o->lru = (LFUGetTimeInMinutes()<<8) | LFU_INIT_VAL  // 低8位频率计数器设初值
        else 
            o->lru = LRU_CLOCK()
### 3、更新
    lookupKey(...)
        if (server.maxmemory_policy & MAXMEMORY_FLAG_LFU)
            updateLFU(val)  // 更新频率计数器
        else 
            val->lru = LRU_CLOCK()

### 4、LRU/LFU算法实现
    evictionPoolPopulate(...) // 随机采样存储数据到pool
    loop 0 -> count: // 随机采样n个(k,v)
        if (server.maxmemory_policy & MAXMEMORY_FLAG_LRU)
            idle = estimateObjectIdleTime(o) // pool中存储server的lru与样本robj的lru的时间差 升序排列
        else if (server.maxmemory_policy & MAXMEMORY_FLAG_LFU)    
            idle = 255-LFUDecrAndReturn(o) // pool中存储(255-robj的频率计数器) 升序排列 保证使用频率最小的对象存储在最后以便淘汰

    LFU的LFUDecrAndReturn计算频率计数器的值
    最终驱逐过期的是采样pool中最末尾的值,对LRU来说是随机采样的count个中时间差最久的(k,v),对LFU来说是随机采用的count个中最低使用频率的(k,v)

### 5、redis中LRU和LFU的应用
    (LRU, LFU) * (allkeys, volatile) 共4中情况
    allkeys-lru     db->dict
    allkeys-lfu     db->dict
    volatile-lru    db->expires
    volatile-lfu    db->expires
    allkeys(db->dict)表示在键空间中采样,volatile(db->expires表示在过期键字典中采样),无论哪种采样,最终的删除要同时作用域键空间dict和过期字典expires

## 三、volatile-ttl淘汰缓存
    volatile(db->expires表示在过期键字典中采样)

## 四、随机淘汰缓存
    random * (allkeys和volatile)共2种情况 
    不采样 直接随机在allkeys或者volatile中获取某一个key,但此淘汰一个
