
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef _NGX_QUEUE_H_INCLUDED_
#define _NGX_QUEUE_H_INCLUDED_

// 如何声明一个带有数据的节点?
// 将ngx_queue_t嵌入另外一个结构体中 比如ngx_http_upstream_keepalive_cache_t
typedef struct ngx_queue_s  ngx_queue_t;

// 队列中定义前向和后向指针 无数据节点
// 使用时需要定义哨兵节点来对队列初始化 以便进行empty()
struct ngx_queue_s {
    ngx_queue_t  *prev;
    ngx_queue_t  *next;
};

// 使用哨兵节点对队列进行初始化
// 哨兵节点的next是队列头
// 哨兵节点的prev是队列尾
#define ngx_queue_init(q)                                                     \
    (q)->prev = q;                                                            \
    (q)->next = q


// 队列判空
#define ngx_queue_empty(h)                                                    \
    (h == (h)->prev)


// 在h后插入x节点
#define ngx_queue_insert_head(h, x)                                           \
    (x)->next = (h)->next;                                                    \
    (x)->next->prev = x;                                                      \
    (x)->prev = h;                                                            \
    (h)->next = x


#define ngx_queue_insert_after   ngx_queue_insert_head


// 在h前插入x节点
#define ngx_queue_insert_tail(h, x)                                           \
    (x)->prev = (h)->prev;                                                    \
    (x)->prev->next = x;                                                      \
    (x)->next = h;                                                            \
    (h)->prev = x


// 头结点
#define ngx_queue_head(h)                                                     \
    (h)->next


// 尾节点
#define ngx_queue_last(h)                                                     \
    (h)->prev


// 哨兵节点
#define ngx_queue_sentinel(h)                                                 \
    (h)


// q的下一个节点
#define ngx_queue_next(q)                                                     \
    (q)->next


// q的前一个节点
#define ngx_queue_prev(q)                                                     \
    (q)->prev


#if (NGX_DEBUG)

#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next;                                              \
    (x)->prev = NULL;                                                         \
    (x)->next = NULL

#else

// 删除x节点
#define ngx_queue_remove(x)                                                   \
    (x)->next->prev = (x)->prev;                                              \
    (x)->prev->next = (x)->next

#endif


#define ngx_queue_split(h, q, n)                                              \
    (n)->prev = (h)->prev;                                                    \
    (n)->prev->next = n;                                                      \
    (n)->next = q;                                                            \
    (h)->prev = (q)->prev;                                                    \
    (h)->prev->next = h;                                                      \
    (q)->prev = n;


#define ngx_queue_add(h, n)                                                   \
    (h)->prev->next = (n)->next;                                              \
    (n)->next->prev = (h)->prev;                                              \
    (h)->prev = (n)->prev;                                                    \
    (h)->prev->next = h;


#define ngx_queue_data(q, type, link)                                         \
    (type *) ((u_char *) q - offsetof(type, link))


ngx_queue_t *ngx_queue_middle(ngx_queue_t *queue);
void ngx_queue_sort(ngx_queue_t *queue,
    ngx_int_t (*cmp)(const ngx_queue_t *, const ngx_queue_t *));


#endif /* _NGX_QUEUE_H_INCLUDED_ */
