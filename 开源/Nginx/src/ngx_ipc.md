
## 信号处理
### ngx_master_process_cycle
    sigemptyset
    sigaddset
    sigprocmask
    sigsuspend
### nginx自定义信号及信号处理函数
    NGX_SHUTDOWN_SIGNAL
    NGX_TERMINATE_SIGNAL
    NGX_NOACCEPT_SIGNAL
    NGX_RECONFIGURE_SIGNAL
    NGX_REOPEN_SIGNAL
    NGX_CHANGEBIN_SIGNAL

    ngx_master_process_cycle

### 如何不终止服务实现配置更新
    

## 进程间通信
### unix域套接字socketpair
    master和worker之间的通信

### 共享内存
    全局共享内存链表shared_memory
    共享内存结构体tag字段
    共享内存管理机制(锁、slab)
    多进程使用如何保证互斥(互斥锁)
    nginx的slab机制(缓存与对齐)
    nginx如何保证共享内存额高性能(slab机制)
    slab机制对内存的两级管理page页与slab块

## fork
    子进程和父进程共享的资源及各自独占的资源
    同一级的子进程间通信(进程间文件描述符的传递)worker间通信
    