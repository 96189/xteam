# redis配置文件
# Scan命令
    迭代数据库的所有键                             SCAN cursor [MATCH pattern] [COUNT count]        
    迭代数据库中key指定的set对象中的元素            SSCAN key cursor [MATCH pattern] [COUNT count]
    迭代数据库中key指定的hash对象中的所有键值对      HSCAN key cursor [MATCH pattern] [COUNT count]
    迭代数据库中key指定的zset中的所有元素           ZSCAN key cursor [MATCH pattern] [COUNT count]

        以上列出的四个命令都支持增量式迭代， 它们每次执行都只会返回少量元素， 所以这些命令可以用于生产环境， 而不会出现像 
    KEYS 命  令、SMEMBERS 命令带来的问题 —— 当 KEYS 命令被用于处理一个大的数据库时， 又或者 SMEMBERS 命令被用于处理一
    个大的集合键时，它们可能会阻塞服务器达数秒之久。

        scan命令指定cursor从0开始迭代,直到返回cursor为0表示结束,每次迭代返回N个元素.
        sscan/hscan/zscan迭代set对象hash对象zset对象时,若set和hash底层编码不是OBJ_ENCODING_HT,zset底层编码不是
    OBJ_ENCODING_SKIPLIST则会一次性返回所有元素,否则返回N个元素

# Redis过期键(expire key)的删除策略
## 一、键过期值的设置及存储
### 底层数据结构
    struct redisDb 
    {
        dict *dict; // 键空间 保存数据库中所有的键值对(添加、删除、更新、取值等基本操作)
        dict *expires; // 过期字典 数据库中所有键的过期时间
        ...
    }

### EXPIRE系列命令设置过期时间
    void expireGenericCommand(client *c, long long basetime, int unit)
    {       
        when += basetime;   // 转绝对时间
        if (lookupKeyWrite(c->db,key))  // 检查key在数据库中的存在性 
        setExpire(c,c->db,key,when)     // 设置过期时间
        {
            kde = dictFind(db->dict,key->ptr)   // 确定key再键空间的存在性
            de = dictAddOrFind(db->expires,dictGetKey(kde))     // 再数据库过期字典中查找或者创建key
            dictSetSignedIntegerVal(de,when)    // 设置数据库过期字典中key对应的过期时间
        }
    }
### 向数据库中插入(k,v)
    set hset lset sass zadd等操作 是只在redisDb::dict键空间执行的,因此这些命令执行后key是没有过期时间的,
    ttlGenericCommand(key) // 返回给定 key 的剩余生存时间
    {
        ttl = -1    // 初始化ttl为-1
        expire = getExpire(c->db,c->argv[1])    // 在redisDb::expires过期字典中查询key的过期时间
        if (expire != -1) 
        {
             ttl = expire-mstime();
        }
    }
    因此 未设置过期时间的key查询ttl时会返回-1
## 二、惰性删除策略(expireIfNeeded) - 被动删除
### 1、被动删除处理流程
    int expireIfNeeded(redisDb *db, robj *key) 
    {
        mstime_t when = getExpire(db,key)   // db->expires
        ...
        if (now > when) dbSyncDelete(db,key)    // 从键空间和过期键字典中同时删除key
    }
    
### 2、执行时机
    任何对key的读写操作都会执行被动删除,相当于把删除操作分摊到每一次的请求中
    
    lookupKeyRead
    lookupKeyWrite
    dbRandomKey
    delGenericCommand
    existsCommand
    keysCommand
    scanGenericCommand

## 三、定期删除策略(activeExpireCycle) - 主动删除
### 1、主动过期处理流程
    void activeExpireCycle(int type) 
    {
        loop : database[0] -> database[15]  // 遍历所有数据库
        {
            do 
            {
                while (num--)   // num最大为ACTIVE_EXPIRE_CYCLE_LOOKUPS_PER_LOOP=20
                {
                    de = dictGetRandomKey(db->expires)  // 获取过期键字典中随机的某个键
                    if (activeExpireCycleTryExpire(db,de,now)) expired++    // 检查当前键是否过期 若过期则从数据库中删除(键空间和过期字典中都删除)
                }
            } while (expired > ACTIVE_EXPIRE_CYCLE_LOOKUPS_PER_LOOP/4);  // 本次loop检测到当前数据库超过指定量的25%过期则继续执行过期 
        }
    }
### 2、何时触发主动删除
    定时执行100ms执行一次
        serverCron -> databasesCron -> activeExpireCycle
    每次事件分派之前执行
        beforeSleep -> activeExpireCycle