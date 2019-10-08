/*
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

#include "server.h"
#include <math.h> /* isnan(), isinf() */

/*-----------------------------------------------------------------------------
 * String Commands
 *----------------------------------------------------------------------------*/

static int checkStringLength(client *c, long long size) {
    if (size > 512*1024*1024) {
        addReplyError(c,"string exceeds maximum allowed size (512MB)");
        return C_ERR;
    }
    return C_OK;
}

/* The setGenericCommand() function implements the SET operation with different
 * options and variants. This function is called in order to implement the
 * following commands: SET, SETEX, PSETEX, SETNX.
 *
 * 'flags' changes the behavior of the command (NX or XX, see belove).
 *
 * 'expire' represents an expire to set in form of a Redis object as passed
 * by the user. It is interpreted according to the specified 'unit'.
 *
 * 'ok_reply' and 'abort_reply' is what the function will reply to the client
 * if the operation is performed, or when it is not because of NX or
 * XX flags.
 *
 * If ok_reply is NULL "+OK" is used.
 * If abort_reply is NULL, "$-1" is used. */

#define OBJ_SET_NO_FLAGS 0
#define OBJ_SET_NX (1<<0)     /* Set if key not exists. */
#define OBJ_SET_XX (1<<1)     /* Set if key exists. */
#define OBJ_SET_EX (1<<2)     /* Set if time in seconds is given */
#define OBJ_SET_PX (1<<3)     /* Set if time in ms in given */
// flags                OBJ_SET_NX  OBJ_SET_NO_FLAGS
// key val              键值
// expire               过期时间
// unit                 UNIT_SECONDS表示秒
// ok_reply abort_reply
// 设置键的值 可能附带一些其他参数
void setGenericCommand(client *c, int flags, robj *key, robj *val, robj *expire, int unit, robj *ok_reply, robj *abort_reply) {
    long long milliseconds = 0; /* initialized to avoid any harmness warning */

    // 过期时间不为0
    if (expire) {
        // 将expire写入milliseconds
        if (getLongLongFromObjectOrReply(c, expire, &milliseconds, NULL) != C_OK)
            return;
        if (milliseconds <= 0) {
            addReplyErrorFormat(c,"invalid expire time in %s",c->cmd->name);
            return;
        }
        // 过期时间单位是秒需要转换为毫秒
        if (unit == UNIT_SECONDS) milliseconds *= 1000;
    }

    if ((flags & OBJ_SET_NX && lookupKeyWrite(c->db,key) != NULL) ||
        (flags & OBJ_SET_XX && lookupKeyWrite(c->db,key) == NULL))
    {
        addReply(c, abort_reply ? abort_reply : shared.nullbulk);
        return;
    }
    // key val存储到数据库
    setKey(c->db,key,val);
    // 
    server.dirty++;
    // 设置key的过期时间
    if (expire) setExpire(c,c->db,key,mstime()+milliseconds);
    // 事件通知
    notifyKeyspaceEvent(NOTIFY_STRING,"set",key,c->db->id);
    if (expire) notifyKeyspaceEvent(NOTIFY_GENERIC,
        "expire",key,c->db->id);
    // 回复客户端
    addReply(c, ok_reply ? ok_reply : shared.ok);
}

/* SET key value [NX] [XX] [EX <seconds>] [PX <milliseconds>] */
void setCommand(client *c) {
    int j;
    robj *expire = NULL;
    int unit = UNIT_SECONDS;
    int flags = OBJ_SET_NO_FLAGS;

    for (j = 3; j < c->argc; j++) {
        char *a = c->argv[j]->ptr;
        robj *next = (j == c->argc-1) ? NULL : c->argv[j+1];

        if ((a[0] == 'n' || a[0] == 'N') &&
            (a[1] == 'x' || a[1] == 'X') && a[2] == '\0' &&
            !(flags & OBJ_SET_XX))
        {
            flags |= OBJ_SET_NX;
        } else if ((a[0] == 'x' || a[0] == 'X') &&
                   (a[1] == 'x' || a[1] == 'X') && a[2] == '\0' &&
                   !(flags & OBJ_SET_NX))
        {
            flags |= OBJ_SET_XX;
        } else if ((a[0] == 'e' || a[0] == 'E') &&
                   (a[1] == 'x' || a[1] == 'X') && a[2] == '\0' &&
                   !(flags & OBJ_SET_PX) && next)
        {
            flags |= OBJ_SET_EX;
            unit = UNIT_SECONDS;
            expire = next;
            j++;
        } else if ((a[0] == 'p' || a[0] == 'P') &&
                   (a[1] == 'x' || a[1] == 'X') && a[2] == '\0' &&
                   !(flags & OBJ_SET_EX) && next)
        {
            flags |= OBJ_SET_PX;
            unit = UNIT_MILLISECONDS;
            expire = next;
            j++;
        } else {
            addReply(c,shared.syntaxerr);
            return;
        }
    }
    // 尝试对字符串对象编码以节约内存
    c->argv[2] = tryObjectEncoding(c->argv[2]);
    setGenericCommand(c,flags,c->argv[1],c->argv[2],expire,unit,NULL,NULL);
}

// SETNX key value
void setnxCommand(client *c) {
    c->argv[2] = tryObjectEncoding(c->argv[2]);
    setGenericCommand(c,OBJ_SET_NX,c->argv[1],c->argv[2],NULL,0,shared.cone,shared.czero);
}

// SETEX key seconds value
void setexCommand(client *c) {
    c->argv[3] = tryObjectEncoding(c->argv[3]);
    setGenericCommand(c,OBJ_SET_NO_FLAGS,c->argv[1],c->argv[3],c->argv[2],UNIT_SECONDS,NULL,NULL);
}

// PSETEX key milliseconds value
void psetexCommand(client *c) {
    c->argv[3] = tryObjectEncoding(c->argv[3]);
    setGenericCommand(c,OBJ_SET_NO_FLAGS,c->argv[1],c->argv[3],c->argv[2],UNIT_MILLISECONDS,NULL,NULL);
}

// 取出键的值
int getGenericCommand(client *c) {
    robj *o;
    // c->argv[1]为key 尝试从数据库中取出键的值
    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.nullbulk)) == NULL)
        return C_OK;

    // 类型检查
    if (o->type != OBJ_STRING) {
        addReply(c,shared.wrongtypeerr);
        return C_ERR;
    } else {
        addReplyBulk(c,o);
        return C_OK;
    }
}

void getCommand(client *c) {
    getGenericCommand(c);
}

// GETSET key value
void getsetCommand(client *c) {
    // 取出键的值对象
    if (getGenericCommand(c) == C_ERR) return;
    // (编码新值)尝试对字符串对象编码以节约内存
    c->argv[2] = tryObjectEncoding(c->argv[2]);
    // 设置key的新值
    setKey(c->db,c->argv[1],c->argv[2]);
    // 事件通知
    notifyKeyspaceEvent(NOTIFY_STRING,"set",c->argv[1],c->db->id);
    server.dirty++;
}

// SETRANGE key offset value
void setrangeCommand(client *c) {
    robj *o;
    long offset;
    sds value = c->argv[3]->ptr;

    // 设置偏移值到offset中
    if (getLongFromObjectOrReply(c,c->argv[2],&offset,NULL) != C_OK)
        return;

    if (offset < 0) {
        addReplyError(c,"offset is out of range");
        return;
    }

    // 查询key对应的val是否存在
    o = lookupKeyWrite(c->db,c->argv[1]);
    // 不存在
    if (o == NULL) {
        /* Return 0 when setting nothing on a non-existing string */
        if (sdslen(value) == 0) {
            addReply(c,shared.czero);
            return;
        }

        /* Return when the resulting string exceeds allowed size */
        // 检查若添加后对象总大小不超过512MB
        if (checkStringLength(c,offset+sdslen(value)) != C_OK)
            return;

        // 创建对象
        o = createObject(OBJ_STRING,sdsnewlen(NULL, offset+sdslen(value)));
        // 添加到数据库
        dbAdd(c->db,c->argv[1],o);
    // 存在
    } else {
        size_t olen;

        /* Key exists, check type */
        if (checkType(c,o,OBJ_STRING))
            return;

        /* Return existing string length when setting nothing */
        olen = stringObjectLen(o);
        if (sdslen(value) == 0) {
            addReplyLongLong(c,olen);
            return;
        }

        /* Return when the resulting string exceeds allowed size */
        // 检查若添加后是否超过512MB
        if (checkStringLength(c,offset+sdslen(value)) != C_OK)
            return;

        /* Create a copy when the object is shared or encoded. */
        o = dbUnshareStringValue(c->db,c->argv[1],o);
    }

    if (sdslen(value) > 0) {
        // sds扩容
        o->ptr = sdsgrowzero(o->ptr,offset+sdslen(value));
        // 拷贝数据
        memcpy((char*)o->ptr+offset,value,sdslen(value));
        signalModifiedKey(c->db,c->argv[1]);
        // 事件通知
        notifyKeyspaceEvent(NOTIFY_STRING,
            "setrange",c->argv[1],c->db->id);
        server.dirty++;
    }
    addReplyLongLong(c,sdslen(o->ptr));
}

// GETRANGE key start end
void getrangeCommand(client *c) {
    robj *o;
    long long start, end;
    char *str, llbuf[32];
    size_t strlen;

    // 保存截取的起始值到start
    if (getLongLongFromObjectOrReply(c,c->argv[2],&start,NULL) != C_OK)
        return;
    // 保存截取的结束值到end
    if (getLongLongFromObjectOrReply(c,c->argv[3],&end,NULL) != C_OK)
        return;
    // 查询key对应的值对象是否存在
    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.emptybulk)) == NULL ||
        checkType(c,o,OBJ_STRING)) return;

    // 如果值对象以整数编码,则需要将整数转字符串
    if (o->encoding == OBJ_ENCODING_INT) {
        str = llbuf;
        strlen = ll2string(llbuf,sizeof(llbuf),(long)o->ptr);
    // raw或enbed编码直接保存值对象
    } else {
        str = o->ptr;
        strlen = sdslen(str);
    }

    /* Convert negative indexes */
    // 范围值合理
    if (start < 0 && end < 0 && start > end) {
        addReply(c,shared.emptybulk);
        return;
    }

    // 修正范围值
    if (start < 0) start = strlen+start;
    if (end < 0) end = strlen+end;
    if (start < 0) start = 0;
    if (end < 0) end = 0;
    if ((unsigned long long)end >= strlen) end = strlen-1;

    /* Precondition: end >= 0 && end < strlen, so the only condition where
     * nothing can be returned is: start > end. */
    if (start > end || strlen == 0) {
        addReply(c,shared.emptybulk);
    } else {
        // 返回指定范围的值
        addReplyBulkCBuffer(c,(char*)str+start,end-start+1);
    }
}

// MGET key [key ...]
void mgetCommand(client *c) {
    int j;

    addReplyMultiBulkLen(c,c->argc-1);
    for (j = 1; j < c->argc; j++) {
        // 查询指定key的val对象是否存在
        robj *o = lookupKeyRead(c->db,c->argv[j]);
        // 不存在
        if (o == NULL) {
            addReply(c,shared.nullbulk);
        // 存在
        } else {
            // 类型检查
            if (o->type != OBJ_STRING) {
                addReply(c,shared.nullbulk);
            } else {
                addReplyBulk(c,o);
            }
        }
    }
}


void msetGenericCommand(client *c, int nx) {
    int j, busykeys = 0;

    // k1 v1 ...
    if ((c->argc % 2) == 0) {
        addReplyError(c,"wrong number of arguments for MSET");
        return;
    }
    /* Handle the NX flag. The MSETNX semantic is to return zero and don't
     * set nothing at all if at least one already key exists. */
    // key不存在才设置
    // 要么不设置 要么都设置
    if (nx) {
        // keyN对应的valueN存在,统计这种清空的个数
        for (j = 1; j < c->argc; j += 2) {
            if (lookupKeyWrite(c->db,c->argv[j]) != NULL) {
                busykeys++;
            }
        }
        // 至少有一个key已经存在余数据库中,直接返回
        if (busykeys) {
            addReply(c, shared.czero);
            return;
        }
    }

    // 设置n个键值对到数据库
    for (j = 1; j < c->argc; j += 2) {
        // (编码新值)尝试对字符串对象编码以节约内存
        c->argv[j+1] = tryObjectEncoding(c->argv[j+1]);
        // 添加到数据库
        setKey(c->db,c->argv[j],c->argv[j+1]);
        // 事件通知
        notifyKeyspaceEvent(NOTIFY_STRING,"set",c->argv[j],c->db->id);
    }
    server.dirty += (c->argc-1)/2;
    addReply(c, nx ? shared.cone : shared.ok);
}

// MSET key value [key value ...]
void msetCommand(client *c) {
    msetGenericCommand(c,0);
}

// MSETNX key value [key value ...]
void msetnxCommand(client *c) {
    msetGenericCommand(c,1);
}

// 增减 或者按照指定值增减都调用此函数
void incrDecrCommand(client *c, long long incr) {
    long long value, oldvalue;
    robj *o, *new;

    // 取出key对应的val
    o = lookupKeyWrite(c->db,c->argv[1]);
    // 值对象存在 检查值对象类型
    if (o != NULL && checkType(c,o,OBJ_STRING)) return;
    // 存储值对象到value
    if (getLongLongFromObjectOrReply(c,o,&value,NULL) != C_OK) return;

    oldvalue = value;

    // 检查是否会溢出
    if ((incr < 0 && oldvalue < 0 && incr < (LLONG_MIN-oldvalue)) ||
        (incr > 0 && oldvalue > 0 && incr > (LLONG_MAX-oldvalue))) {
        addReplyError(c,"increment or decrement would overflow");
        return;
    }

    // 增量增加
    // 减量减少
    value += incr;

    // 整数字符串 且原来的空间足以存放新值
    if (o && o->refcount == 1 && o->encoding == OBJ_ENCODING_INT &&
        (value < 0 || value >= OBJ_SHARED_INTEGERS) &&
        value >= LONG_MIN && value <= LONG_MAX)
    {
        new = o;
        // 再原地址存放新值
        o->ptr = (void*)((long)value);
    } else {
        // 根据新值创建新的对象
        new = createStringObjectFromLongLong(value);
        // 值对象已存在则覆盖
        if (o) {
            dbOverwrite(c->db,c->argv[1],new);
        // 对象不存在则添加
        } else {
            dbAdd(c->db,c->argv[1],new);
        }
    }
    // 
    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(NOTIFY_STRING,"incrby",c->argv[1],c->db->id);
    server.dirty++;

    //
    addReply(c,shared.colon);
    addReply(c,new);
    addReply(c,shared.crlf);
}

// INCR key
void incrCommand(client *c) {
    incrDecrCommand(c,1);
}

// DECR key
void decrCommand(client *c) {
    incrDecrCommand(c,-1);
}

// INCRBY key increment
void incrbyCommand(client *c) {
    long long incr;

    // 取出增量
    if (getLongLongFromObjectOrReply(c, c->argv[2], &incr, NULL) != C_OK) return;
    incrDecrCommand(c,incr);
}

// DECRBY key decrement
void decrbyCommand(client *c) {
    long long incr;

    // 取出减量
    if (getLongLongFromObjectOrReply(c, c->argv[2], &incr, NULL) != C_OK) return;
    incrDecrCommand(c,-incr);
}

// INCRBYFLOAT key increment
void incrbyfloatCommand(client *c) {
    long double incr, value;
    robj *o, *new, *aux;

    // 获取key对那个的val
    o = lookupKeyWrite(c->db,c->argv[1]);
    // 值存在 类型检查
    if (o != NULL && checkType(c,o,OBJ_STRING)) return;
    // 将值存储到value中
    // 将需要增加的增量存储到incr中
    if (getLongDoubleFromObjectOrReply(c,o,&value,NULL) != C_OK ||
        getLongDoubleFromObjectOrReply(c,c->argv[2],&incr,NULL) != C_OK)
        return;

    // 增量自增
    value += incr;

    // 检查溢出
    if (isnan(value) || isinf(value)) {
        addReplyError(c,"increment would produce NaN or Infinity");
        return;
    }

    // 使用新值创建新的对象
    new = createStringObjectFromLongDouble(value,1);
    // 值对象存在 用旧的覆盖新的
    if (o)
        dbOverwrite(c->db,c->argv[1],new);
    // 值对象不存在 直接添加
    else
        dbAdd(c->db,c->argv[1],new);

    // 
    signalModifiedKey(c->db,c->argv[1]);
    notifyKeyspaceEvent(NOTIFY_STRING,"incrbyfloat",c->argv[1],c->db->id);
    server.dirty++;
    addReplyBulk(c,new);

    /* Always replicate INCRBYFLOAT as a SET command with the final value
     * in order to make sure that differences in float precision or formatting
     * will not create differences in replicas or after an AOF restart. */
    aux = createStringObject("SET",3);
    rewriteClientCommandArgument(c,0,aux);
    decrRefCount(aux);
    rewriteClientCommandArgument(c,2,new);
}

// APPEND key value
void appendCommand(client *c) {
    size_t totlen;
    robj *o, *append;

    // 取出键对应的值对象
    o = lookupKeyWrite(c->db,c->argv[1]);
    // 键没有找到对应值(key不在数据库中)
    if (o == NULL) {
        /* Create the key */
        // 创建一个值对象
        c->argv[2] = tryObjectEncoding(c->argv[2]);
        // 添加键值对
        dbAdd(c->db,c->argv[1],c->argv[2]);
        // 引用计数自增
        incrRefCount(c->argv[2]);
        // 计算新值的长度
        totlen = stringObjectLen(c->argv[2]);
    // 键对应有值
    } else {
        /* Key exists, check type */
        if (checkType(c,o,OBJ_STRING))
            return;

        /* "append" is an argument, so always an sds */
        append = c->argv[2];
        // 计算新值的长度
        totlen = stringObjectLen(o)+sdslen(append->ptr);
        // 检查新值是否唱过长度上限512MB
        if (checkStringLength(c,totlen) != C_OK)
            return;

        /* Append the value */
        o = dbUnshareStringValue(c->db,c->argv[1],o);
        // 添加到原sds对象末尾
        o->ptr = sdscatlen(o->ptr,append->ptr,sdslen(append->ptr));
        totlen = sdslen(o->ptr);
    }
    // 向数据库发送键被修改的信号
    signalModifiedKey(c->db,c->argv[1]);
    // 事件通知
    notifyKeyspaceEvent(NOTIFY_STRING,"append",c->argv[1],c->db->id);
    server.dirty++;
    addReplyLongLong(c,totlen);
}

// STRLEN key
void strlenCommand(client *c) {
    robj *o;
    // 取出值对象并进行类型检查
    if ((o = lookupKeyReadOrReply(c,c->argv[1],shared.czero)) == NULL ||
        checkType(c,o,OBJ_STRING)) return;
    // 对值求长度
    addReplyLongLong(c,stringObjectLen(o));
}
