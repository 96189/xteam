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
## 一、惰性删除策略(expireIfNeeded)
    1、执行时机
        任何对key的读写操作

## 二、定期删除策略(activeExpireCycle)
    2、执行时机    
        serverCron -> databasesCron -> activeExpireCycle
        beforeSleep -> activeExpireCycle