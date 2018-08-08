# 缓存淘汰策略(内存释放)
    https://redis.io/topics/lru-cache
  
## 一、执行时机
    freeMemoryIfNeeded()      // 命令执行时,检查内存是否够用

## 二、LRU/LFU结合allkeys和volatile数据集淘汰缓存
    LFU详细 http://antirez.com/news/109
### 1、LRU/LFU实现原理
    typedef struct redisObject {
        unsigned type:4;
        unsigned encoding:4;
        unsigned lru:LRU_BITS; /* LRU time (relative to global lru_clock) or
                                * LFU data (least significant 8 bits frequency
                                * and most significant 16 bits access time). */
        int refcount;
        void *ptr;
    } robj;

### 2、初始化
    createObject(...)
        o->lru = LRU_CLOCK()
        o->lru = (LFUGetTimeInMinutes()<<8) | LFU_INIT_VAL

### 3、更新
    lookupKey(...)
        val->lru = LRU_CLOCK()
        updateLFU(val)

### 4、LRU/LFU算法实现
    evictionPoolPopulate(...) // 随机采样存储数据到pool
        LRU
            pool中存储server的lru与样本robj的lru的时间差 升序排列
        LFU
            pool中存储(255-robj的频率计数器) 升序排列 保证使用频率最小的对象存储在最后以便淘汰

### 5、redis中LRU和LFU的应用
    (LRU, LFU) * (allkeys, volatile) 共4中情况
    allkeys-lru     db->dict
    allkeys-lfu     db->dict
    volatile-lru    db->expires
    volatile-lfu    db->expires

## 三、volatile-ttl淘汰缓存

## 四、allkeys-random和volatile-random淘汰缓存
