
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>


// pool 分配内存使用的pool
// n 每个节点固定长度的数组长度
// size 存放具体元素的个数
ngx_list_t *
ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    ngx_list_t  *list;

    list = ngx_palloc(pool, sizeof(ngx_list_t));
    if (list == NULL) {
        return NULL;
    }

    if (ngx_list_init(list, pool, n, size) != NGX_OK) {
        return NULL;
    }

    return list;
}


// 在给定list尾部追加一个元素 并返回新元素存放空间的指针
void *
ngx_list_push(ngx_list_t *l)
{
    void             *elt;
    ngx_list_part_t  *last;

    last = l->last;

    // 最后一个节点满
    if (last->nelts == l->nalloc) {

        /* the last part is full, allocate a new list part */

        last = ngx_palloc(l->pool, sizeof(ngx_list_part_t));
        if (last == NULL) {
            return NULL;
        }

        // 申请新的空间
        last->elts = ngx_palloc(l->pool, l->nalloc * l->size);
        if (last->elts == NULL) {
            return NULL;
        }

        last->nelts = 0;
        last->next = NULL;

        // 构成链表
        l->last->next = last;
        l->last = last;
    }

    // 新元素地址
    elt = (char *) last->elts + l->size * last->nelts;
    // 元素个数累加
    last->nelts++;

    return elt;
}
