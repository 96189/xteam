
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HASH_H_INCLUDED_
#define _NGX_HASH_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct {
    void             *value;
    u_short           len;
    u_char            name[1];
} ngx_hash_elt_t;


// 普通hash表
typedef struct {
    ngx_hash_elt_t  **buckets;
    ngx_uint_t        size;
} ngx_hash_t;


// 带通配符的hash表
typedef struct {
    ngx_hash_t        hash;
    void             *value;
} ngx_hash_wildcard_t;


typedef struct {
    ngx_str_t         key;
    ngx_uint_t        key_hash;     // hash(key) = key_hash
    void             *value;
} ngx_hash_key_t;


typedef ngx_uint_t (*ngx_hash_key_pt) (u_char *data, size_t len);


// 组合hash表
typedef struct {
    // 普通hash表
    ngx_hash_t            hash;
    // 前置通配符hash表
    ngx_hash_wildcard_t  *wc_head;
    // 后置通配符hash表
    ngx_hash_wildcard_t  *wc_tail;
} ngx_hash_combined_t;

// 初始化参数集合
typedef struct {
    ngx_hash_t       *hash;         // hash表指针 为NULL表示最终指向新生成的hash表 非NULL将所有数据插入到指定的hash表
    ngx_hash_key_pt   key;          // 字符串 -> hash值 的hash函数

    ngx_uint_t        max_size;     // hash表中桶的个数 桶越多 每个桶中存放的元素个数越少 查询速度更快
    ngx_uint_t        bucket_size;  // 每个桶的最大限制(单位字节) 初始化hash表时,发现某个桶无法存放所有属于该桶的元素 初始化失败

    char             *name;         // 该hash表的名字
    ngx_pool_t       *pool;         // 该hash表分配内存使用的pool
    ngx_pool_t       *temp_pool;    // 临时pool 初始化完成后 本字段对应的pool可以被释放和销毁
} ngx_hash_init_t;


#define NGX_HASH_SMALL            1
#define NGX_HASH_LARGE            2

#define NGX_HASH_LARGE_ASIZE      16384
#define NGX_HASH_LARGE_HSIZE      10007

#define NGX_HASH_WILDCARD_KEY     1
#define NGX_HASH_READONLY_KEY     2


// 构建ngx_hash_combined_t的辅助类型
// 根据keys、dns_wc_head、dns_wc_tail是否为空来决定是否构建普通hash表 前置通配符hash表 后置通配符hash表
typedef struct {
    ngx_uint_t        hsize;            // 桶的个数

    ngx_pool_t       *pool;             // 构建这些hash表使用的pool
    ngx_pool_t       *temp_pool;        // 临时pool 三个hash表构建完成后 此指针指向的pool可被销毁

    ngx_array_t       keys;             // 存放所有非通配符的数组
    ngx_array_t      *keys_hash;        // 保存和检测keys是否有重复

    ngx_array_t       dns_wc_head;      // 存放前置通配符key被处理以后的值
    ngx_array_t      *dns_wc_head_hash; // 保存和检测dns_wc_head是否有重复

    ngx_array_t       dns_wc_tail;      // 存放后置通配符key被处理以后的值
    ngx_array_t      *dns_wc_tail_hash; // 保存和检测dns_wc_tail是否有重复
} ngx_hash_keys_arrays_t;


typedef struct {
    ngx_uint_t        hash;
    ngx_str_t         key;
    ngx_str_t         value;
    u_char           *lowcase_key;
} ngx_table_elt_t;


void *ngx_hash_find(ngx_hash_t *hash, ngx_uint_t key, u_char *name, size_t len);
void *ngx_hash_find_wc_head(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_wc_tail(ngx_hash_wildcard_t *hwc, u_char *name, size_t len);
void *ngx_hash_find_combined(ngx_hash_combined_t *hash, ngx_uint_t key,
    u_char *name, size_t len);

ngx_int_t ngx_hash_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);
ngx_int_t ngx_hash_wildcard_init(ngx_hash_init_t *hinit, ngx_hash_key_t *names,
    ngx_uint_t nelts);

#define ngx_hash(key, c)   ((ngx_uint_t) key * 31 + c)
ngx_uint_t ngx_hash_key(u_char *data, size_t len);
ngx_uint_t ngx_hash_key_lc(u_char *data, size_t len);
ngx_uint_t ngx_hash_strlow(u_char *dst, u_char *src, size_t n);


ngx_int_t ngx_hash_keys_array_init(ngx_hash_keys_arrays_t *ha, ngx_uint_t type);
ngx_int_t ngx_hash_add_key(ngx_hash_keys_arrays_t *ha, ngx_str_t *key,
    void *value, ngx_uint_t flags);


#endif /* _NGX_HASH_H_INCLUDED_ */
