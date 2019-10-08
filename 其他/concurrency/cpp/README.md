
https://zh.cppreference.com/w/cpp/thread

defer_lock_t	不获得互斥的所有权
try_to_lock_t	尝试获得互斥的所有权而不阻塞
adopt_lock_t	假设调用方线程已拥有互斥的所有权

// 互斥
mutex

// 互斥封装器
lock_guard
unique_lock

// 锁定给定的可锁对象 内部免死锁算法避免多个互斥量的死锁
lock

