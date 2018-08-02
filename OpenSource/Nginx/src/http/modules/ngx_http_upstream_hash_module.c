
/*
 * Copyright (C) Roman Arutyunyan
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// 虚拟节点
typedef struct {
    // 虚拟节点的hash值
    uint32_t                            hash;
    // 虚拟节点归属的真实节点 对应真实节点的server成员
    ngx_str_t                          *server;
} ngx_http_upstream_chash_point_t;


// 虚拟节点数组
typedef struct {
    // 虚拟节点的个数
    ngx_uint_t                          number;
    // 虚拟节点的数组
    ngx_http_upstream_chash_point_t     point[1];
} ngx_http_upstream_chash_points_t;


// server配置
typedef struct {
    // 关联hash指令的第一个参数 用于计算请求的hash值
    ngx_http_complex_value_t            key;
    // 虚拟节点的数组
    ngx_http_upstream_chash_points_t   *points;
} ngx_http_upstream_hash_srv_conf_t;


// 请求的一致性哈希数据
typedef struct {
    /* the round robin data must be first */
    // round robin的per request负载均衡数据
    ngx_http_upstream_rr_peer_data_t    rrp;
    // server配置块
    ngx_http_upstream_hash_srv_conf_t  *conf;
    // 对于本次请求 hash指令的第一个参数的具体值 用于计算本次请求的哈希值
    ngx_str_t                           key;
    // 已经尝试的虚拟节点数
    ngx_uint_t                          tries;
    // 
    ngx_uint_t                          rehash;
    // 根据请求的哈希值 找到顺时方向最近的一个虚拟节点 hash为该虚拟节点在数组中的索引
    uint32_t                            hash;
    // round robin算法的peer.get函数
    ngx_event_get_peer_pt               get_rr_peer;
} ngx_http_upstream_hash_peer_data_t;


static ngx_int_t ngx_http_upstream_init_hash(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_init_hash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_hash_peer(ngx_peer_connection_t *pc,
    void *data);

static ngx_int_t ngx_http_upstream_init_chash(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us);
static int ngx_libc_cdecl
    ngx_http_upstream_chash_cmp_points(const void *one, const void *two);
static ngx_uint_t ngx_http_upstream_find_chash_point(
    ngx_http_upstream_chash_points_t *points, uint32_t hash);
static ngx_int_t ngx_http_upstream_init_chash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_chash_peer(ngx_peer_connection_t *pc,
    void *data);

static void *ngx_http_upstream_hash_create_conf(ngx_conf_t *cf);
static char *ngx_http_upstream_hash(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_hash_commands[] = {

    { ngx_string("hash"),
      NGX_HTTP_UPS_CONF|NGX_CONF_TAKE12,
      ngx_http_upstream_hash,
      NGX_HTTP_SRV_CONF_OFFSET,
      0,
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_upstream_hash_module_ctx = {
    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    ngx_http_upstream_hash_create_conf,    /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_upstream_hash_module = {
    NGX_MODULE_V1,
    &ngx_http_upstream_hash_module_ctx,    /* module context */
    ngx_http_upstream_hash_commands,       /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_int_t
ngx_http_upstream_init_hash(ngx_conf_t *cf, ngx_http_upstream_srv_conf_t *us)
{
    if (ngx_http_upstream_init_round_robin(cf, us) != NGX_OK) {
        return NGX_ERROR;
    }

    us->peer.init = ngx_http_upstream_init_hash_peer;

    return NGX_OK;
}


// hash算法的per request负载均衡初始化函数
static ngx_int_t
ngx_http_upstream_init_hash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    ngx_http_upstream_hash_srv_conf_t   *hcf;
    ngx_http_upstream_hash_peer_data_t  *hp;

    hp = ngx_palloc(r->pool, sizeof(ngx_http_upstream_hash_peer_data_t));
    if (hp == NULL) {
        return NGX_ERROR;
    }

    r->upstream->peer.data = &hp->rrp;

    // 调用round robin的per request负载均衡初始化函数
    if (ngx_http_upstream_init_round_robin_peer(r, us) != NGX_OK) {
        return NGX_ERROR;
    }

    r->upstream->peer.get = ngx_http_upstream_get_hash_peer;

    hcf = ngx_http_conf_upstream_srv_conf(us, ngx_http_upstream_hash_module);

    // 获取本请求对应的hash指令的第一个参数值 用于计算请求的hash值
    if (ngx_http_complex_value(r, &hcf->key, &hp->key) != NGX_OK) {
        return NGX_ERROR;
    }

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "upstream hash key:\"%V\"", &hp->key);

    hp->conf = hcf;
    hp->tries = 0;
    hp->rehash = 0;
    hp->hash = 0;
    // round robin的peer.get函数
    hp->get_rr_peer = ngx_http_upstream_get_round_robin_peer;

    return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_get_hash_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_hash_peer_data_t  *hp = data;

    time_t                        now;
    u_char                        buf[NGX_INT_T_LEN];
    size_t                        size;
    uint32_t                      hash;
    ngx_int_t                     w;
    uintptr_t                     m;
    ngx_uint_t                    n, p;
    ngx_http_upstream_rr_peer_t  *peer;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get hash peer, try: %ui", pc->tries);

    ngx_http_upstream_rr_peers_wlock(hp->rrp.peers);

    if (hp->tries > 20 || hp->rrp.peers->single) {
        ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
        return hp->get_rr_peer(pc, &hp->rrp);
    }

    now = ngx_time();

    pc->cached = 0;
    pc->connection = NULL;

    for ( ;; ) {

        /*
         * Hash expression is compatible with Cache::Memcached:
         * ((crc32([REHASH] KEY) >> 16) & 0x7fff) + PREV_HASH
         * with REHASH omitted at the first iteration.
         */

        ngx_crc32_init(hash);

        if (hp->rehash > 0) {
            size = ngx_sprintf(buf, "%ui", hp->rehash) - buf;
            ngx_crc32_update(&hash, buf, size);
        }

        ngx_crc32_update(&hash, hp->key.data, hp->key.len);
        ngx_crc32_final(hash);

        hash = (hash >> 16) & 0x7fff;

        hp->hash += hash;
        hp->rehash++;

        w = hp->hash % hp->rrp.peers->total_weight;
        peer = hp->rrp.peers->peer;
        p = 0;

        while (w >= peer->weight) {
            w -= peer->weight;
            peer = peer->next;
            p++;
        }

        n = p / (8 * sizeof(uintptr_t));
        m = (uintptr_t) 1 << p % (8 * sizeof(uintptr_t));

        if (hp->rrp.tried[n] & m) {
            goto next;
        }

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "get hash peer, value:%uD, peer:%ui", hp->hash, p);

        if (peer->down) {
            goto next;
        }

        if (peer->max_fails
            && peer->fails >= peer->max_fails
            && now - peer->checked <= peer->fail_timeout)
        {
            goto next;
        }

        break;

    next:

        if (++hp->tries > 20) {
            ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
            return hp->get_rr_peer(pc, &hp->rrp);
        }
    }

    hp->rrp.current = peer;

    pc->sockaddr = peer->sockaddr;
    pc->socklen = peer->socklen;
    pc->name = &peer->name;

    peer->conns++;

    if (now - peer->checked > peer->fail_timeout) {
        peer->checked = now;
    }

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    hp->rrp.tried[n] |= m;

    return NGX_OK;
}


// 使用一致性哈希初始化upstream块
static ngx_int_t
ngx_http_upstream_init_chash(ngx_conf_t *cf, ngx_http_upstream_srv_conf_t *us)
{
    u_char                             *host, *port, c;
    size_t                              host_len, port_len, size;
    uint32_t                            hash, base_hash;
    ngx_str_t                          *server;
    ngx_uint_t                          npoints, i, j;
    ngx_http_upstream_rr_peer_t        *peer;
    ngx_http_upstream_rr_peers_t       *peers;
    ngx_http_upstream_chash_points_t   *points;
    ngx_http_upstream_hash_srv_conf_t  *hcf;
    union {
        uint32_t                        value;
        u_char                          byte[4];
    } prev_hash;

    // 使用round robin的upstream块初始化函数 创建和初始化真实节点
    if (ngx_http_upstream_init_round_robin(cf, us) != NGX_OK) {
        return NGX_ERROR;
    }

    // 重新设置per request的负载均衡初始化函数
    us->peer.init = ngx_http_upstream_init_chash_peer;

    // 真实节点的集群
    peers = us->peer.data;
    // 虚拟节点的总个数
    npoints = peers->total_weight * 160;

    size = sizeof(ngx_http_upstream_chash_points_t)
           + sizeof(ngx_http_upstream_chash_point_t) * (npoints - 1);

    // 创建npoints个虚拟节点
    points = ngx_palloc(cf->pool, size);
    if (points == NULL) {
        return NGX_ERROR;
    }

    points->number = 0;

    // 初始化所有的虚拟节点
    // 遍历后端服务器列表
    for (peer = peers->peer; peer; peer = peer->next) {
        server = &peer->server;

        /*
         * Hash expression is compatible with Cache::Memcached::Fast:
         * crc32(HOST \0 PORT PREV_HASH).
         */

        if (server->len >= 5
            && ngx_strncasecmp(server->data, (u_char *) "unix:", 5) == 0)
        {
            host = server->data + 5;
            host_len = server->len - 5;
            port = NULL;
            port_len = 0;
            goto done;
        }

        // 把每个peer的server成员解析为host和port
        for (j = 0; j < server->len; j++) {
            c = server->data[server->len - j - 1];

            if (c == ':') {
                host = server->data;
                host_len = server->len - j - 1;
                port = server->data + server->len - j;
                port_len = j;
                goto done;
            }

            if (c < '0' || c > '9') {
                break;
            }
        }

        host = server->data;
        host_len = server->len;
        port = NULL;
        port_len = 0;

    done:

        // 根据解析peer的server成员所得的HOST和PORT 计算虚拟节点的base_hash值
        ngx_crc32_init(base_hash);
        ngx_crc32_update(&base_hash, host, host_len);
        ngx_crc32_update(&base_hash, (u_char *) "", 1);
        ngx_crc32_update(&base_hash, port, port_len);

        // 对于归属同一个真实节点的虚拟节点 它们的base_hash值相同 而prev_hash不同
        // 第一个节点的prev_hash为0
        prev_hash.value = 0;
        // 当前后端服务器真实节点对应的虚拟节点个数
        npoints = peer->weight * 160;

        // 初始化当前后端真实节点对应的虚拟节点
        for (j = 0; j < npoints; j++) {
            hash = base_hash;

            ngx_crc32_update(&hash, prev_hash.byte, 4);
            ngx_crc32_final(hash);

            // 虚拟节点的哈希值
            points->point[points->number].hash = hash;
            // 虚拟节点所归属的真实节点 对应真实节点的server成员
            points->point[points->number].server = server;
            points->number++;

#if (NGX_HAVE_LITTLE_ENDIAN)
            prev_hash.value = hash;
#else
            prev_hash.byte[0] = (u_char) (hash & 0xff);
            prev_hash.byte[1] = (u_char) ((hash >> 8) & 0xff);
            prev_hash.byte[2] = (u_char) ((hash >> 16) & 0xff);
            prev_hash.byte[3] = (u_char) ((hash >> 24) & 0xff);
#endif
        }
    }

    // 使用快速排序 使虚拟节点数组的元素 按照其hash值从小到大有序
    ngx_qsort(points->point,
              points->number,
              sizeof(ngx_http_upstream_chash_point_t),
              ngx_http_upstream_chash_cmp_points);

    // 如果虚拟节点数组中 有多个元素的hash值相同 只保留第一个
    for (i = 0, j = 1; j < points->number; j++) {
        if (points->point[i].hash != points->point[j].hash) {
            points->point[++i] = points->point[j];
        }
    }

    // 经过上述步骤后 虚拟节点数组中的元素 有序而不重复

    points->number = i + 1;

    hcf = ngx_http_conf_upstream_srv_conf(us, ngx_http_upstream_hash_module);
    // 保存虚拟节点数组
    hcf->points = points;

    return NGX_OK;
}


static int ngx_libc_cdecl
ngx_http_upstream_chash_cmp_points(const void *one, const void *two)
{
    ngx_http_upstream_chash_point_t *first =
                                       (ngx_http_upstream_chash_point_t *) one;
    ngx_http_upstream_chash_point_t *second =
                                       (ngx_http_upstream_chash_point_t *) two;

    if (first->hash < second->hash) {
        return -1;

    } else if (first->hash > second->hash) {
        return 1;

    } else {
        return 0;
    }
}


// 二分查找 寻找第一个hash值大于等于请求的哈希值的虚拟节点 即“顺时针方向最近”的一个虚拟节点
static ngx_uint_t
ngx_http_upstream_find_chash_point(ngx_http_upstream_chash_points_t *points,
    uint32_t hash)
{
    ngx_uint_t                        i, j, k;
    ngx_http_upstream_chash_point_t  *point;

    /* find first point >= hash */

    point = &points->point[0];

    i = 0;
    j = points->number;

    while (i < j) {
        k = (i + j) / 2;

        if (hash > point[k].hash) {
            i = k + 1;

        } else if (hash < point[k].hash) {
            j = k;

        } else {
            return k;
        }
    }

    return i;
}


// 初始化请求的负载均衡数据
static ngx_int_t
ngx_http_upstream_init_chash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    uint32_t                             hash;
    ngx_http_upstream_hash_srv_conf_t   *hcf;
    ngx_http_upstream_hash_peer_data_t  *hp;

    // 调用hash算法的per request负载均衡初始化函数 创建和初始化请求的负载均衡数据
    if (ngx_http_upstream_init_hash_peer(r, us) != NGX_OK) {
        return NGX_ERROR;
    }

    // 重新指定peer.get 用于选取一个真实节点
    r->upstream->peer.get = ngx_http_upstream_get_chash_peer;

    hp = r->upstream->peer.data;
    hcf = ngx_http_conf_upstream_srv_conf(us, ngx_http_upstream_hash_module);

    // 根据获取的本请求对应的hash指令的第一个参数值 计算请求的hash值
    hash = ngx_crc32_long(hp->key.data, hp->key.len);

    ngx_http_upstream_rr_peers_rlock(hp->rrp.peers);

    // 根据请求的hash值 找到顺时针方向最近的一个虚拟节点 hp->hash记录此虚拟节点
    // 在数组中的索引
    hp->hash = ngx_http_upstream_find_chash_point(hcf->points, hash);

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    return NGX_OK;
}


// 选取一个真实的节点
static ngx_int_t
ngx_http_upstream_get_chash_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_hash_peer_data_t  *hp = data;

    time_t                              now;
    intptr_t                            m;
    ngx_str_t                          *server;
    ngx_int_t                           total;
    ngx_uint_t                          i, n, best_i;
    ngx_http_upstream_rr_peer_t        *peer, *best;
    ngx_http_upstream_chash_point_t    *point;
    ngx_http_upstream_chash_points_t   *points;
    ngx_http_upstream_hash_srv_conf_t  *hcf;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get consistent hash peer, try: %ui", pc->tries);

    ngx_http_upstream_rr_peers_wlock(hp->rrp.peers);

    pc->cached = 0;
    pc->connection = NULL;

    now = ngx_time();
    hcf = hp->conf;

    // 虚拟节点数组
    points = hcf->points;
    // 指向第一个虚拟节点
    point = &points->point[0];

    for ( ;; ) {
        // 在peer.init中，已根据请求的哈希值，找到顺时针方向最近的一个虚拟节点，
        // hash为该虚拟节点在数组中的索引。
        // 一开始hash值肯定小于number，之后每尝试一个虚拟节点后，hash++。取模是为了防止越界访问
        server = point[hp->hash % points->number].server;

        ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "consistent hash peer:%uD, server:\"%V\"",
                       hp->hash, server);

        best = NULL;
        best_i = 0;
        total = 0;

        // 遍历真实节点数组，寻找可用的、该虚拟节点归属的真实节点(server成员相同)
        // 如果有多个真实节点同时符合条件 那么使用轮询来从中选取一个真实节点
        for (peer = hp->rrp.peers->peer, i = 0;
             peer;
             peer = peer->next, i++)
        {

            // 检查此真实节点在状态位图中对应的位 为1时表示不可用
            n = i / (8 * sizeof(uintptr_t));
            m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

            if (hp->rrp.tried[n] & m) {
                continue;
            }

            // server指令中携带了down属性 表示后端宕机
            if (peer->down) {
                continue;
            }

            // 如果真实节点的server成员和虚拟节点的不同 表示虚拟节点不属于此真实节点
            if (peer->server.len != server->len
                || ngx_strncmp(peer->server.data, server->data, server->len)
                   != 0)
            {
                continue;
            }

            // 在一段时间内 如果此真实节点的失败次数 超过了允许的最大值 那么不允许使用了
            if (peer->max_fails
                && peer->fails >= peer->max_fails
                && now - peer->checked <= peer->fail_timeout)
            {
                continue;
            }

            // 对每个真实节点 增加其当前权重
            peer->current_weight += peer->effective_weight;
            // 累加所有真实节点的有效权重
            total += peer->effective_weight;

            // 如果之前此真实节点发生了失败 会减小其effective_weight来降低它的权重          
            // 此后又通过增加其effective_weight来恢复它的权重
            if (peer->effective_weight < peer->weight) {
                peer->effective_weight++;
            }

            // 选取当前权重最大者 作为本次选定的真实节点
            if (best == NULL || peer->current_weight > best->current_weight) {
                best = peer;
                best_i = i;
            }
        }

        // 如果选定了一个真实节点
        if (best) {
            // 如果使用了轮询 需要降低选定节点的当前权重
            best->current_weight -= total;
            goto found;
        }

        // 增加虚拟节点的索引 即“沿着顺时针方向”
        hp->hash++;
        // 已经尝试的虚拟节点数
        hp->tries++;

        // 如果把所有的虚拟节点都尝试了一遍 还找不到可用的真实节点
        if (hp->tries >= points->number) {
            ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);
            return NGX_BUSY;
        }
    }

// 找到了和虚拟节点相对应的、可用的真实节点了
found:

    // 选定的真实节点
    hp->rrp.current = best;

    // 保存选定的后端服务器的地址 之后会向这个地址发起连接
    pc->sockaddr = best->sockaddr;
    pc->socklen = best->socklen;
    pc->name = &best->name;

    best->conns++;

    // 更新checked时间
    if (now - best->checked > best->fail_timeout) {
        best->checked = now;
    }

    ngx_http_upstream_rr_peers_unlock(hp->rrp.peers);

    // 对于本次请求 如果之后需要再次选取真实节点 不能再选取同一个了
    n = best_i / (8 * sizeof(uintptr_t));
    m = (uintptr_t) 1 << best_i % (8 * sizeof(uintptr_t));

    hp->rrp.tried[n] |= m;

    return NGX_OK;
}


static void *
ngx_http_upstream_hash_create_conf(ngx_conf_t *cf)
{
    ngx_http_upstream_hash_srv_conf_t  *conf;

    conf = ngx_palloc(cf->pool, sizeof(ngx_http_upstream_hash_srv_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    conf->points = NULL;

    return conf;
}


// hash指令的解析函数
// hash $host$uri 使用哈希算法
// hash $host$uri consistent 使用一致性哈希算法
static char *
ngx_http_upstream_hash(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_upstream_hash_srv_conf_t  *hcf = conf;

    ngx_str_t                         *value;
    ngx_http_upstream_srv_conf_t      *uscf;
    ngx_http_compile_complex_value_t   ccv;

    value = cf->args->elts;

    ngx_memzero(&ccv, sizeof(ngx_http_compile_complex_value_t));

    // 把hash指令的第一个参数，关联到一个ngx_http_complex_value_t变量
    // 之后可以通过该变量获取参数的实时值
    ccv.cf = cf;
    ccv.value = &value[1];
    ccv.complex_value = &hcf->key;

    if (ngx_http_compile_complex_value(&ccv) != NGX_OK) {
        return NGX_CONF_ERROR;
    }

    // 获取所在的upstream{}块
    uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);

    if (uscf->peer.init_upstream) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "load balancing method redefined");
    }

    // 指定此upstream块中server指令支持的属性
    uscf->flags = NGX_HTTP_UPSTREAM_CREATE
                  |NGX_HTTP_UPSTREAM_WEIGHT
                  |NGX_HTTP_UPSTREAM_MAX_FAILS
                  |NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
                  |NGX_HTTP_UPSTREAM_DOWN;

    // 根据hash指令携带的参数来判断是使用哈希算法 还是一致性哈希算法。
    // 每种算法都有自己的upstream块初始化函数
    if (cf->args->nelts == 2) {
        uscf->peer.init_upstream = ngx_http_upstream_init_hash;

    } else if (ngx_strcmp(value[2].data, "consistent") == 0) {
        uscf->peer.init_upstream = ngx_http_upstream_init_chash;

    } else {
        ngx_conf_log_error(NGX_LOG_EMERG, cf, 0,
                           "invalid parameter \"%V\"", &value[2]);
        return NGX_CONF_ERROR;
    }

    return NGX_CONF_OK;
}
