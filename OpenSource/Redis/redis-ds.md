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
    hash函数生成算法 siphash https://en.wikipedia.org/wiki/SipHash
    hash冲突解决:链地址法
    hash表的扩展与收缩
    redis的渐进式rehash算法(重新散列算法)

## 六、字典
    哈希表实现字典

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