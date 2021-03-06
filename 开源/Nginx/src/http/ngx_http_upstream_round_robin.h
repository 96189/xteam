
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_
#define _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


typedef struct ngx_http_upstream_rr_peer_s   ngx_http_upstream_rr_peer_t;

// 描述后端服务器节点(单个节点的数据结构)
struct ngx_http_upstream_rr_peer_s {
    // 后端服务器的地址
    struct sockaddr                *sockaddr;
    // 地址的长度
    socklen_t                       socklen;
    // 后端服务器地址的字符串
    ngx_str_t                       name;
    // server的名称
    ngx_str_t                       server;

    // 当前权重 初始值为0 动态调整
    ngx_int_t                       current_weight;
    // 有效权重 初始为weight会因失败而降低
    ngx_int_t                       effective_weight;
    // 配置权重 固定不变
    ngx_int_t                       weight;

    // 当前连接数
    ngx_uint_t                      conns;

    // 一段时间内已经失败的次数
    ngx_uint_t                      fails;
    // 最近一次失败的时间点
    time_t                          accessed;
    // 用于检查是否超过了一段时间
    time_t                          checked;

    // 一段时间 最大的失败次数 固定值
    ngx_uint_t                      max_fails;
    // 一段时间的值 固定值
    time_t                          fail_timeout;

    // 服务器宕机标志
    ngx_uint_t                      down;          /* unsigned  down:1; */

#if (NGX_HTTP_SSL)
    void                           *ssl_session;
    int                             ssl_session_len;
#endif

    // 下一个服务器节点
    ngx_http_upstream_rr_peer_t    *next;

#if (NGX_HTTP_UPSTREAM_ZONE)
    ngx_atomic_t                    lock;
#endif
};


typedef struct ngx_http_upstream_rr_peers_s  ngx_http_upstream_rr_peers_t;

// 描述后端服务器列表(n个节点的数据结构)
// 可能是主机列表peer
// 也可能是备机列表next
struct ngx_http_upstream_rr_peers_s {
    // 当前列表中的后端服务器数量
    ngx_uint_t                      number;

#if (NGX_HTTP_UPSTREAM_ZONE)
    ngx_slab_pool_t                *shpool;
    ngx_atomic_t                    rwlock;
    ngx_http_upstream_rr_peers_t   *zone_next;
#endif

    // 当前所有服务器节点的总权值
    ngx_uint_t                      total_weight;

    // 为1标识当前列表中只有1个节点
    unsigned                        single:1;
    // 是否使用权重
    unsigned                        weighted:1;

    // upstream配置块的名称
    ngx_str_t                      *name;

    // 后端备机服务器列表地址(指向连续的一块内存)
    ngx_http_upstream_rr_peers_t   *next;

    // 后端主机服务器列表地址(指向连续的一块内存)
    ngx_http_upstream_rr_peer_t    *peer;
};


#if (NGX_HTTP_UPSTREAM_ZONE)

#define ngx_http_upstream_rr_peers_rlock(peers)                               \
                                                                              \
    if (peers->shpool) {                                                      \
        ngx_rwlock_rlock(&peers->rwlock);                                     \
    }

#define ngx_http_upstream_rr_peers_wlock(peers)                               \
                                                                              \
    if (peers->shpool) {                                                      \
        ngx_rwlock_wlock(&peers->rwlock);                                     \
    }

#define ngx_http_upstream_rr_peers_unlock(peers)                              \
                                                                              \
    if (peers->shpool) {                                                      \
        ngx_rwlock_unlock(&peers->rwlock);                                    \
    }


#define ngx_http_upstream_rr_peer_lock(peers, peer)                           \
                                                                              \
    if (peers->shpool) {                                                      \
        ngx_rwlock_wlock(&peer->lock);                                        \
    }

#define ngx_http_upstream_rr_peer_unlock(peers, peer)                         \
                                                                              \
    if (peers->shpool) {                                                      \
        ngx_rwlock_unlock(&peer->lock);                                       \
    }

#else

#define ngx_http_upstream_rr_peers_rlock(peers)
#define ngx_http_upstream_rr_peers_wlock(peers)
#define ngx_http_upstream_rr_peers_unlock(peers)
#define ngx_http_upstream_rr_peer_lock(peers, peer)
#define ngx_http_upstream_rr_peer_unlock(peers, peer)

#endif


typedef struct {
    // 后端主机备机服务器列表
    ngx_http_upstream_rr_peers_t   *peers;
    // 被选中的服务器
    ngx_http_upstream_rr_peer_t    *current;
    // 位图地址指向data 或者指向堆分配的大空间
    uintptr_t                      *tried;
    // 被赋值为0
    uintptr_t                       data;
} ngx_http_upstream_rr_peer_data_t;


ngx_int_t ngx_http_upstream_init_round_robin(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
ngx_int_t ngx_http_upstream_init_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
ngx_int_t ngx_http_upstream_create_round_robin_peer(ngx_http_request_t *r,
    ngx_http_upstream_resolved_t *ur);
ngx_int_t ngx_http_upstream_get_round_robin_peer(ngx_peer_connection_t *pc,
    void *data);
void ngx_http_upstream_free_round_robin_peer(ngx_peer_connection_t *pc,
    void *data, ngx_uint_t state);

#if (NGX_HTTP_SSL)
ngx_int_t
    ngx_http_upstream_set_round_robin_peer_session(ngx_peer_connection_t *pc,
    void *data);
void ngx_http_upstream_save_round_robin_peer_session(ngx_peer_connection_t *pc,
    void *data);
#endif


#endif /* _NGX_HTTP_UPSTREAM_ROUND_ROBIN_H_INCLUDED_ */
