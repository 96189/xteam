/*
 * Copyright (c) 2009-2012, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 * Copyright (c) 2009-2012, Salvatore Sanfilippo <antirez at gmail dot com>
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

#ifndef __INTSET_H
#define __INTSET_H
#include <stdint.h>

// 整数集合作为集合键的底层实现之一
// 有序不重复的整数数组
// 支持int16_t int32_t int64_t整数值
// 64位: 8Byte
typedef struct intset {
    // 编码方式INTSET_ENC_INT16 INTSET_ENC_INT32 INTSET_ENC_INT64
    uint32_t encoding;
    // 数组长度(元素个数)
    uint32_t length;
    // 整数数组,具体保存什么类型的整数取决于encoding字段
    int8_t contents[];
} intset;

// 创建整数集合
intset *intsetNew(void);
// 添加指定元素到整数集合 O(n)
intset *intsetAdd(intset *is, int64_t value, uint8_t *success);
// 从整数集合中删除指定元素 O(n)
intset *intsetRemove(intset *is, int64_t value, int *success);
// 检查指定元素是否在整数集合 O(logN) 有序数组 二分查找
uint8_t intsetFind(intset *is, int64_t value);
// 从整数集合中随机返回一个元素
int64_t intsetRandom(intset *is);
// 获取整数集合在指定位置上的整数
uint8_t intsetGet(intset *is, uint32_t pos, int64_t *value);
// 获取整数集合的长度
uint32_t intsetLen(const intset *is);
// 获取整数集合占用的字节数
size_t intsetBlobLen(intset *is);

#ifdef REDIS_TEST
int intsetTest(int argc, char *argv[]);
#endif

#endif // __INTSET_H
