
##  mysql
+   常见问题
    *   key和index的区别

        索引(index)负责维护表的查找和操作速度

        约束(key)负责维护表的完整性

        约束有 主键约束、外键约束、非空约束、检查约束等

        索引有 普通索引、主键索引、唯一索引、联合索引、全文索引等

        理论上有差异,实际上没有区别.

+   存储引擎
    *   InnoDB

        事务 行锁 外键
    *   MyISAM

        全文索引
+   索引
    *   底层分类
        *   B+Tree索引
            *   聚集索引(Clustered index)

                好处:对于主键的排序查找非常的快,对于主键的范围查找非常的快
            *   辅助索引(Secondary index)

                检索两遍索引：首先检索辅助索引获得主键,然后用主键到主索引中检索获得记录.因为辅助索引不会对影响数据在聚集索引中的组织,所以可以有多个.
        *   哈希索引

            memory（内存）存储引擎 哈希索引不支持范围查找和排序的功能
        *   空间数据索引(R-Tree)
        *   全文索引

            MyISAM和InnoDB
    *   逻辑分类
        *   普通索引(INDEX or KEY)
        *   唯一性索引(UNIQUE)
        *   主键索引(PRIMARY KEY)
        *   空间索引
        *   全文索引(FULLTEXT)

            只能在CHAR、VARCHAR或TEXT类型上创建

##  Q&A
+   什么时候使用索引?

    表的主关键字

    直接条件查询的字段

    查询中与其它表关联的字段

    查询中排序的字段

    查询中统计或分组统计的字段

+ PRIMARY KEY 和 UNIQUE KEY 的区别

    PRIMARY KEYs（主键） 和 UNIQUE KEYs（唯一键约束） 是类似的， PRIMARY KEY 通常是一列，也有可能多列，通常由他来决定一行数据 (row)。 一张表只能有一个 PRIMARY KEY，但可以有很多 UNIQUE KEY。 当给一列设置为 UNIQUE KEY 之后，不能有两行在该列上有相同的数据。 PRIMARY KEY 不允许有 NULL 值，但是 UNIQUE KEY 可以。

##   相关文章
+   [mysql索引的新手入门详解](https://blog.csdn.net/u012954706/article/details/81241049)
+   [MySQL索引背后的数据结构及算法原理](http://blog.codinglabs.org/articles/theory-of-mysql-index.html)
+   [从 MongoDB 及 Mysql 谈B/B+树](https://blog.csdn.net/wwh578867817/article/details/50493940)
