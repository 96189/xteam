tc_lock.h
    锁模板TC_LockT
    空锁TC_EmptyMutex
    读锁模板TC_RW_RLockT
    写锁模板TC_RW_WLockT

tc_thread_mutex.h
    线程锁(不可循环加锁)TC_ThreadMutex
    线程锁(可循环加锁内部计数实现)TC_ThreadRecMutex

tc_thread_rwlock.h
    读写锁TC_ThreadRWLocker

tc_thread_cond.h
    条件变量TC_ThreadCond

tc_monitor.h
    管程的c++实现
    线程锁监控模板TC_Monitor
    内部数据成员锁、条件变量、加锁次数

应用中使用的线程锁都是通过以下两个模板类型来定义的
/**
 * 普通线程锁
 */
typedef TC_Monitor<TC_ThreadMutex, TC_ThreadCond> TC_ThreadLock;

/**
 * 循环锁(一个线程可以加多次锁)
 */
typedef TC_Monitor<TC_ThreadRecMutex, TC_ThreadCond> TC_ThreadRecLock;

tc_thread_queue.h
    线程安全的队列(模板类)

tc_thread_pool.h
    线程池
        工作线程组和任务队列