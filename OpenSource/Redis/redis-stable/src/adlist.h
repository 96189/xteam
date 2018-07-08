/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */
// 双向链表节点
// 32位:sizeof(listNode) = 12
// 64位:sizeof(listNode) = 24
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

// 双向链表迭代器
typedef struct listIter {
    listNode *next;
    int direction;
} listIter;

// 双向链表结构
// 32位:sizeof(list) = 24
// 64位:sizeof(list) = 48
typedef struct list {
    listNode *head;
    listNode *tail;
    // 表节点复制函数
    void *(*dup)(void *ptr);
    // 表节点释放函数
    void (*free)(void *ptr);
    // 表节点值对比函数
    int (*match)(void *ptr, void *key);
    // 链表长度32位4Byte 64位8Byte
    unsigned long len;
} list;

/* Functions implemented as macros */
// llen
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

// 设置节点复制函数
#define listSetDupMethod(l,m) ((l)->dup = (m))
// 设置节点释放函数
#define listSetFreeMethod(l,m) ((l)->free = (m))
// 设置节点比较函数
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
list *listCreate(void);
// 释放链表节点 释放链表结构 O(n)
void listRelease(list *list);
// 清空链表 销毁所有节点 O(n)
void listEmpty(list *list);
// 头插法 O(1) lpush
list *listAddNodeHead(list *list, void *value);
// 尾插法 O(1) rpush
list *listAddNodeTail(list *list, void *value);
// 在list链表的old_node的或前或后插入节点 after非0则在后插入 after为0则在前插入
// O(1) linsert
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
// 删除节点 O(1) lrem lpop rpop
void listDelNode(list *list, listNode *node);
// 创建链表迭代器(正向迭代或者反向迭代)
listIter *listGetIterator(list *list, int direction);
// 迭代器应用 向前或者向后指 O(1)
listNode *listNext(listIter *iter);
// 释放迭代器内存
void listReleaseIterator(listIter *iter);
// 链表复制 O(n)
list *listDup(list *orig);
// 在链表中搜索指定的key O(n)
listNode *listSearchKey(list *list, void *key);
// 索引链表正向或者反向的第n个元素 O(n)
// index > 0正向 < 0 反向
// 索引计数从0开始
// lset lindex
listNode *listIndex(list *list, long index);
// 创建正向迭代器 O(1)
void listRewind(list *list, listIter *li);
// 创建反向迭代器 O(1)
void listRewindTail(list *list, listIter *li);
// 取出链表的表尾节点，并将它移动到表头，成为新的表头节点 O(1)
void listRotate(list *list);
// 连接链表o到链表l O(1)
void listJoin(list *l, list *o);

/* Directions for iterators */
// 迭代器方向 从头到尾
#define AL_START_HEAD 0
// 迭代器方向 从尾到头
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
