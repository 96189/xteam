
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

+   索引
    *   底层分类
        *   B+Tree索引
            *   聚集索引(Clustered index)

                好处:对于主键的排序查找非常的快,对于主键的范围查找非常的快
            *   辅助索引(Secondary index)

                检索两遍索引：首先检索辅助索引获得主键,然后用主键到主索引中检索获得记录.因为辅助索引不会对影响数据在聚集索引中的组织,所以可以有多个.
        *   哈希索引
        *   空间数据索引(R-Tree)
        *   全文索引
    *   逻辑分类
        *   普通索引(INDEX or KEY)
        *   唯一性索引(UNIQUE)
        *   主键索引(PRIMARY KEY)
        *   空间索引
        *   全文索引(FULLTEXT)

            只能在CHAR、VARCHAR或TEXT类型上创建


##   相关文章
+   [mysql索引的新手入门详解](https://blog.csdn.net/u012954706/article/details/81241049)
+   [MySQL索引背后的数据结构及算法原理](http://blog.codinglabs.org/articles/theory-of-mysql-index.html)
+   [从 MongoDB 及 Mysql 谈B/B+树](https://blog.csdn.net/wwh578867817/article/details/50493940)
