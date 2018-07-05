
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
    ziplist如何做到节省内存？
