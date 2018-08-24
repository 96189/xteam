# 底层数据结构
## 一、sds字符串对象
### 1、sds对象设计的几个优点
        (1) 常数复杂度获取长度
        (2) 杜绝缓冲区溢出
        (3) 减少修改字符串长度时所需的内存重新分配次数
        (4) 二进制安全
        (5) 兼容部分c函数

## 二、双向链表
    双向链表常见的操作实现
    
## 三、Quicklist
    ziplist和双向链表的结合(列表对象实际使用的数据结构)
### 1、ziplist的优缺点:
        内存连续,插入和删除需要频繁拷贝数据且每次申请新节点内存需要realloc拷贝数据,节点之间的关系只需要较少的额外内存就可以
    维护,但也正是由于内存空间连续内存碎片会小一些
### 2、双向链表的优缺点:
        内存不连续,大量节点的申请和释放会造成比较大的内存碎片且维护节点之间的关系需要大量额外的内存,但也正是由于内存不连续,会
    更灵活的组织数据插入新节点不必拷贝数据
### 3、为什么要引入quicklist:
        集合ziplist和双向链表的优点,通过链表来管理数据,对数据进行分片,每一片数据由内存连续的ziplist管理.极端情况下,若每个
    ziplist中保存一个数据,则quicklist退化为双向链表,若所有数据都存储在一个节点中,则quicklist退化为ziplist。因此需要配置每
    个ziplist中最大可存储多大数据.
## 四、跳跃表
    [skiplist论文](ftp://ftp.cs.umd.edu/pub/skipLists/skiplists.pdf)
    平均O(logN)复杂度的证明 
    幂次定律随机生成层高
    redis跳跃表的几个特点(score可相同,后退指针)

## 五、哈希表
    hash函数的设计原则
        确定性 随机性 均匀性 
    murmurhash2算法
    hash函数生成算法 siphash https://en.wikipedia.org/wiki/SipHash
    hash冲突解决:链地址法
    hash表的扩展与收缩
    

### 1、哈希表节点(桶中元素类型)
    struct dictEntry 
    {
        void *key;  // 键
        union       // 值
        {     
            void *val;
            uint64_t u64;
            int64_t s64;
            double d;
        } v;
        struct dictEntry *next; // 链式结构(构成桶单元) 链地址法解决hash冲突 头插法O(1)效率
    }

### 2、哈希表
    struct dictht 
    {
        dictEntry **table;  // 表项数组(桶单元数组)
        unsigned long size; // 数组大小(桶单元个数)
        unsigned long sizemask; // 大小掩码 计算索引值(桶单元) 值为(size-1)
        unsigned long used; // 所有表项挂载的节点总数
    }

    hash(key)函数的O(1)查找时间复杂度,是建立在hash表地址空间M足够大,足以映射数据集R,假设常用到的数据集为N
    则存在 N < M >> R,实际应用中N和R都是增长的但M无法实时改变,因此随着数据规模的不断增长,必然导致hash冲突加剧,连地址法无法保证查询效率,因此为了提高数据的访问效率,需要增大M重新对数据进行哈希(即rehash)

    ht[0].used >= ht[0].size 意味着哈希地址空间M漫射 有可能需要扩展哈希地址空间
    used/size为哈希空间的装填因子(利用率) 漫射时需要扩张 保持N < M 利用率不足时需要收缩空间
    无论时扩张还是收缩 都需要经过rehash重新散列

## 六、字典(symbol table/associative array/map)
### 1、基础数据结构
    用途:存储key-value-pair
    底层实现:2个哈希表和相关的hash函数
    字典:
    struct dict 
    {
        dictType *type; // 键值操作函数集合
        void *privdata;
        dictht ht[2];   // 哈希表数组 一般只使用ht[0] ht[1]在对哈希表进行rehash时使用
        long rehashidx; // rehash索引
        ... 
    }
    ht[1]相当于备用表,当数据过多时,由于桶个数较少 每一个桶单元内的元素必然线性增多 桶单元内的查找等操作效率必然线性增长,此时需要更大的地址空间,将较多的数据尽量映射均匀.也就是rehash来解决哈希冲突.

### 2、redis的渐进式rehash
    若是由于数据量增长ht[0].used >= ht[0].size且ht[0].used/ht[0].size>dict_force_resize_ratio,则是将ht[0]小数据集迁移到更大的数据集ht[1],是一种哈希空间扩张
    若是由于哈希空间利用率不足ht[0].used/ht[0].size < 10% 则是将较大数据集迁移ht[0]迁移到较小数据集ht[1]中,哈希空间收缩

#### rehash的执行过程
    int dictRehash(dict *d, int n)
    {
        while (n--)
        {
            de = d->ht[0].table[d->rehashidx];  // 定位待迁移数据位置
            while(de)
            {
                h = dictHashKey(d, de->key) & d->ht[1].sizemask // 执行hash算法 定义rehash后的数据位置
                d->ht[1].table[h] = de  // 数据迁移到指定位置
                de = nextde  // 迭代下一个数据
            }
        }
        if (d->ht[0].used == 0) // ht[0]的rehash数据迁移完毕
        {
            d->ht[0] = d->ht[1] // 备用表切换为正在使用表
        }
    }

#### rehash操作的ht[1]空间是何时分配的
    dictAddRaw  // 向hash表中添加元素
        | ->_dictKeyIndex   // 检查是否需要扩展hash表
                | -> _dictExpandIfNeeded    // (d->ht[0].used >= d->ht[0].size && (dict_can_resize || d->ht[0].used/d->ht[0].size > dict_force_resize_ratio)
                                | -> dictExpand


#### 何为渐进式rehash -> 分散(均摊)rehash操作到n个操作中而不是集中一次性rehash
##### 定时rehash
    databasesCron
        | -> incrementallyRehash
                    | -> dictRehashMilliseconds
                                    | -> dictRehash

##### 数据库增删改查操作过程中rehash
    dictAddRaw          // 增
    dictGenericDelete   // 删
    dictFind            // 查
    dictGetRandomKey    // 随机获取
    dictGetSomeKeys     // 获取
            | -> _dictRehashStep
                        | -> dictRehash

### 3、尝试缩小字典
    定时尝试缩小字典
    databasesCron
        | -> tryResizeHashTables
                    | 若字典利用率ht[0].used/ht[0].size < 10%则以下缩小字典
                    | -> dictResize(db->dict)
                    | -> dictResize(db->expires)

## 七、整数集合

## 八、ziplist压缩列表
### 1、空白 ziplist 示例图
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
### 2、非空 ziplist 示例图
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


### 3、ziplist如何做到节省内存？
    ziplist的内存是连续的
    entry-n的内存布局:
        1 or 5字节编码前置节点的长度 | 1 or 2 or 5字节编码当前节点值的长度 | 值

    
# redis对象
    redis对象系统使用void decrRefCount(robj *o)接口实现了基于引用计数的内存回收机制,使用void incrRefCount(robj *o)接口通过引用计数实现了对象共享机制.
## 一、redis接口定义
    typedef struct redisObject {
        unsigned type:4;
        unsigned encoding:4;
        unsigned lru:LRU_BITS; 
        int refcount;
        void *ptr; // 具体的底层数据结构
    } robj;

## 二、字符串对象
### 底层数据结构
    sds
    可存储整数字符串 浮点型字符串 普通字符串
    
## 三、列表对象
### 底层数据结构
    quicklist = list + ziplist
    
## 四、集合对象
### 底层数据结构
    hash table
    intset
    
## 五、哈希对象
### 底层数据结构
    hash table
    ziplist
    
## 六、有序集合对象
### 底层数据结构
    hash table+skiplist
    ziplist
    
## 七、Module对象
### 底层数据结构