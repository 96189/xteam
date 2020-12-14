
# 并发
## 关于并发

    https://cs.lmu.edu/~ray/notes/introconcurrency/

    https://web.mit.edu/6.005/www/fa14/classes/17-concurrency/

    https://sceweb.uhcl.edu/helm/RationalUnifiedProcess/process/workflow/ana_desi/co_cncry.htm#

* keyword

    并行 同步和异步 阻塞和非阻塞

    https://github.com/pinguo/php-msf-docs/blob/master/chapter-2/2.4-%E5%BC%82%E6%AD%A5%E3%80%81%E5%B9%B6%E5%8F%91.md

    https://zhuanlan.zhihu.com/p/31851833

# 多线程编程

## 多线程基本理论
### 需要回答的问题

* 为什么需要多线程？多线程解决什么问题？

    效率 接口时间效率 CPU利用效率 多核(不同CPU并行)和单核(IO和CPU计算并行)

* 多线程编程需要注意的问题？

    尽量消除竞争条件、降低临界区

* 业界优化开源软件中如何使用多线程？

### 多线程同步机制

* 互斥锁

    互斥性，同一时间有竞争的临界区只能被一个线程访问(串行化)，保障状态的正确性

* 条件变量

    阻塞和唤醒

* 信号量

    共享资源计数器

* 线程专有数据

# 在编程中应用
## C&c++并发编程
### c多线程

    https://begriffs.com/posts/2020-03-23-concurrent-programming.html

### c++11以的后并发编程

    https://www.modernescpp.com/images/stories/pdfs/ConcurrencyWithModernC++.pdf



* keyword

    promise

    future

### boost并发编程

## python并发编程

    https://realpython.com/python-concurrency/

    https://www.toptal.com/python/beginners-guide-to-concurrency-and-parallelism-in-python

    https://chriskiehl.com/article/parallelism-in-one-line

