
# sds字符串对象:
    sds对象设计的几个优点
        1、常数复杂度获取长度
        2、杜绝缓冲区溢出
        3、减少修改字符串长度时所需的内存重新分配次数
        4、二进制安全
        5、兼容部分c函数
***
# 双向链表:
    双向链表常见的操作实现
***
# 跳跃表:
    [skiplist论文][ftp://ftp.cs.umd.edu/pub/skipLists/skiplists.pdf]
    平均O(logN)复杂度的证明 
    幂次定律随机生成层高
    redis跳跃表的几个特点(score可相同,后退指针)
***
# 哈希表:
    hash函数生成算法 MurmurHash2
    hash冲突解决:链地址法
    hash表的扩展与收缩
    redis的渐进式rehash算法(重新散列算法)
***
# 字典:
    redis中字段的使用哈希表实现
***
# ziplist压缩列表
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
                                ZIPLIST_ENTRY_HEAD                |   ZIPLIST_ENTRY_END
                                                                  |
                                                        ZIPLIST_ENTRY_TAIL
                                                        
Ziplist 是为了尽可能地节约内存而设计的特殊编码双端链表,Ziplist 允许在列表的两端进行 O(1) 复杂度的 push 和 pop 操作。



    ziplist如何做到节省内存？
