
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_PALLOC_H_INCLUDED_
#define _NGX_PALLOC_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


/*
 * NGX_MAX_ALLOC_FROM_POOL should be (ngx_pagesize - 1), i.e. 4095 on x86.
 * On Windows NT it decreases a number of locked pages in a kernel.
 */
#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       16
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)


typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;  // 函数指针 释放资源的函数
    void                 *data;     // 节点对应的资源
    ngx_pool_cleanup_t   *next;     // 链表下一个元素
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

// 大块内存管理节点(每一个大块内存配备一个该管理节点)
struct ngx_pool_large_s {
    ngx_pool_large_t     *next;     // 下一个大块内存管理节点
    void                 *alloc;    // 大块内存地址
};

// 维护内存池数据块
typedef struct {
    u_char               *last;     // 内存池中可分配内存的起始地址
    u_char               *end;      // 内存池结束地址
    ngx_pool_t           *next;     // 下一个内存管理单元(构成链表)
    ngx_uint_t            failed;   // 当前内存池空间不够分配次数
} ngx_pool_data_t;

// ngx_pool_t
// 内存池管理单元(同时维护大块和小块内存)
struct ngx_pool_s {
    ngx_pool_data_t       d;        // 内存池数据管理节点
    size_t                max;      // 小块内存的最大值(不包含管理节点所占的内存空间)
    ngx_pool_t           *current;  // 当前内存池管理单元(寻址方便) 
    ngx_chain_t          *chain;    // 链表
    ngx_pool_large_t     *large;    // 析构函数,挂载内存释放时需要清理资源的一些必要操作
    ngx_pool_cleanup_t   *cleanup;  // 资源释放链表 链表每一项记录着需要释放的资源 通过这个机制可以管理文件资源
    ngx_log_t            *log;
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
    ngx_log_t            *log;
} ngx_pool_cleanup_file_t;

// 申请内存不初始化
void *ngx_alloc(size_t size, ngx_log_t *log);
// 申请内存后初始化为0
void *ngx_calloc(size_t size, ngx_log_t *log);

// 创建一个内存池管理单元
ngx_pool_t *ngx_create_pool(size_t size, ngx_log_t *log);
// 销毁指定的内存池管理单元
void ngx_destroy_pool(ngx_pool_t *pool);
// 重置指定的内存池管理单元
void ngx_reset_pool(ngx_pool_t *pool);

// 内存申请
// (小块内存或大块内存申请)内部执行对齐
void *ngx_palloc(ngx_pool_t *pool, size_t size);
// 内存申请(不对齐)
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
// 内存申请(对齐内存)并初始化为0
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);

// 申请大内存按照alignment执行对齐,并申请大内存管理节点,将该节点挂载到内存池管理单元
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
// 释放大内存
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);

// 内部申请一个ngx_pool_cleanup_t对象,挂载到内存池管理单元p的cleanup链表上
// size指示ngx_pool_cleanup_t对象的data大小
// 用户调用本函数之后,需要设置返回对象的handler和data
ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
// 关闭指定的文件
void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
// 文件相关
void ngx_pool_cleanup_file(void *data);
// 文件相关
void ngx_pool_delete_file(void *data);


#endif /* _NGX_PALLOC_H_INCLUDED_ */
