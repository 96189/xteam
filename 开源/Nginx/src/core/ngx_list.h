
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_LIST_H_INCLUDED_
#define _NGX_LIST_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


typedef struct ngx_list_part_s  ngx_list_part_t;

// 链表节点定义
struct ngx_list_part_s {
    void             *elts;     // 数组起始地址
    ngx_uint_t        nelts;    // 数组中已有元素个数
    ngx_list_part_t  *next;     // 下一个节点指针
};


// 类似redis中的quick list
// 链表结构 每个链表节点是固定大小的数组
typedef struct {
    ngx_list_part_t  *last;     // 指向链表的第一个节点
    ngx_list_part_t   part;     // 链表中第一个存放具体元素的节点
    size_t            size;     // 链表中存放数据所需要的内存大小
    ngx_uint_t        nalloc;   // 节点中固定大小数组的最大元素个数(容量)
    ngx_pool_t       *pool;     // list使用的分配内存的pool
} ngx_list_t;


ngx_list_t *ngx_list_create(ngx_pool_t *pool, ngx_uint_t n, size_t size);

// 
// pool 分配内存使用的pool
// n 每个节点固定的数组的长度
// size 存放具体元素的个数
static ngx_inline ngx_int_t
ngx_list_init(ngx_list_t *list, ngx_pool_t *pool, ngx_uint_t n, size_t size)
{
    // 申请数组空间
    list->part.elts = ngx_palloc(pool, n * size);
    if (list->part.elts == NULL) {
        return NGX_ERROR;
    }

    // 数组中已使用元素个数初始置为0
    list->part.nelts = 0;
    // 当前节点是最后一个节点 下一个节点为NULL
    list->part.next = NULL;
    // last指向最后一个节点
    list->last = &list->part;
    
    list->size = size;
    list->nalloc = n;
    list->pool = pool;

    return NGX_OK;
}


/*
 *
 *  the iteration through the list:
 *
 *  part = &list.part;
 *  data = part->elts;
 *
 *  for (i = 0 ;; i++) {
 *
 *      if (i >= part->nelts) {
 *          if (part->next == NULL) {
 *              break;
 *          }
 *
 *          part = part->next;
 *          data = part->elts;
 *          i = 0;
 *      }
 *
 *      ...  data[i] ...
 *
 *  }
 */


void *ngx_list_push(ngx_list_t *list);


#endif /* _NGX_LIST_H_INCLUDED_ */
