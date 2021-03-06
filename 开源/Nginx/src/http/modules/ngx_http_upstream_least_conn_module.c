
/*
 * Copyright (C) Maxim Dounin
 * Copyright (C) Nginx, Inc.
 */


#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


static ngx_int_t ngx_http_upstream_init_least_conn_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us);
static ngx_int_t ngx_http_upstream_get_least_conn_peer(
    ngx_peer_connection_t *pc, void *data);
static char *ngx_http_upstream_least_conn(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);


static ngx_command_t  ngx_http_upstream_least_conn_commands[] = {

    { ngx_string("least_conn"),
      NGX_HTTP_UPS_CONF|NGX_CONF_NOARGS,
      ngx_http_upstream_least_conn,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_http_module_t  ngx_http_upstream_least_conn_module_ctx = {
    NULL,                                  /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configuration */
    NULL                                   /* merge location configuration */
};


ngx_module_t  ngx_http_upstream_least_conn_module = {
    NGX_MODULE_V1,
    &ngx_http_upstream_least_conn_module_ctx, /* module context */
    ngx_http_upstream_least_conn_commands, /* module directives */
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
ngx_http_upstream_init_least_conn(ngx_conf_t *cf,
    ngx_http_upstream_srv_conf_t *us)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, cf->log, 0,
                   "init least conn");

    // 使用round robin的upstream块初始化函数 创建和初始化后端集群
    if (ngx_http_upstream_init_round_robin(cf, us) != NGX_OK) {
        return NGX_ERROR;
    }

    // 重新设置per request的负载均衡初始化函数
    us->peer.init = ngx_http_upstream_init_least_conn_peer;

    return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_init_least_conn_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
                   "init least conn peer");

    // 调用round robin的per request负载均衡初始化函数
    if (ngx_http_upstream_init_round_robin_peer(r, us) != NGX_OK) {
        return NGX_ERROR;
    }

    // 指定peer.get 用于从集群中选取一台后端 
    r->upstream->peer.get = ngx_http_upstream_get_least_conn_peer;

    return NGX_OK;
}


static ngx_int_t
ngx_http_upstream_get_least_conn_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_rr_peer_data_t  *rrp = data;

    time_t                         now;
    uintptr_t                      m;
    ngx_int_t                      rc, total;
    ngx_uint_t                     i, n, p, many;
    ngx_http_upstream_rr_peer_t   *peer, *best;
    ngx_http_upstream_rr_peers_t  *peers;

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get least conn peer, try: %ui", pc->tries);

    // 如果集群只包含一台后端 那么就不用选了
    if (rrp->peers->single) {
        return ngx_http_upstream_get_round_robin_peer(pc, rrp);
    }

    pc->cached = 0;
    pc->connection = NULL;

    now = ngx_time();

    // 后端集群
    peers = rrp->peers;

    ngx_http_upstream_rr_peers_wlock(peers);

    best = NULL;
    total = 0;

#if (NGX_SUPPRESS_WARN)
    many = 0;
    p = 0;
#endif

    // 遍历后端集群
    for (peer = peers->peer, i = 0;
         peer;
         peer = peer->next, i++)
    {

        // 检查此后端在状态位图中对应的位 为1时表示不可用
        n = i / (8 * sizeof(uintptr_t));
        m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

        if (rrp->tried[n] & m) {
            continue;
        }

        // 宕机
        if (peer->down) {
            continue;
        }

        // 在一段时间内 如果此后端服务器的失败次数 超过了允许的最大值 那么不允许使用此后端了
        if (peer->max_fails
            && peer->fails >= peer->max_fails
            && now - peer->checked <= peer->fail_timeout)
        {
            continue;
        }

        /*
         * select peer with least number of connections; if there are
         * multiple peers with the same number of connections, select
         * based on round-robin
         */

        // 比较各个后端的conns*weight 选取最小者
        if (best == NULL
            || peer->conns * best->weight < best->conns * peer->weight)
        {
            best = peer;
            many = 0;
            p = i;

        // 如果有多个最小者 设置many标志
        } else if (peer->conns * best->weight == best->conns * peer->weight) {
            many = 1;
        }
    }

    // 如果有多个后端的conns*weight同为最小者 则对它们使用轮询算法
    if (best == NULL) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "get least conn peer, no peer found");

        goto failed;
    }

    if (many) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "get least conn peer, many");

        for (peer = best, i = p;
             peer;
             peer = peer->next, i++)
        {
            n = i / (8 * sizeof(uintptr_t));
            m = (uintptr_t) 1 << i % (8 * sizeof(uintptr_t));

            if (rrp->tried[n] & m) {
                continue;
            }

            if (peer->down) {
                continue;
            }

            // best时conns * wight最小的服务器节点
            // 跳过不是最小的conns * weight
            if (peer->conns * best->weight != best->conns * peer->weight) {
                continue;
            }

            if (peer->max_fails
                && peer->fails >= peer->max_fails
                && now - peer->checked <= peer->fail_timeout)
            {
                continue;
            }

            // 对每个最小conn * weight后端 增加其当前权重
            peer->current_weight += peer->effective_weight;
            total += peer->effective_weight;

            // 如果之前此后端发生了失败 会减小其effective_weight来降低它的权重         
            // 此后在选取后端的过程中 又通过增加其effective_weight来恢复它的权重
            if (peer->effective_weight < peer->weight) {
                peer->effective_weight++;
            }

            // 选取当前权重最大者 作为本次选定的后端
            if (peer->current_weight > best->current_weight) {
                best = peer;
                p = i;
            }
        }
    }

    // 如果使用轮询 要降低选定后端的当前权重
    best->current_weight -= total;

    if (now - best->checked > best->fail_timeout) {
        best->checked = now;
    }

    pc->sockaddr = best->sockaddr;
    pc->socklen = best->socklen;
    pc->name = &best->name;

    // 增加选定后端的当前连接数
    best->conns++;

    rrp->current = best;

    // 对于此请求 如果之后需要再次选取后端 不能再选取这个后端了
    n = p / (8 * sizeof(uintptr_t));
    m = (uintptr_t) 1 << p % (8 * sizeof(uintptr_t));

    rrp->tried[n] |= m;

    ngx_http_upstream_rr_peers_unlock(peers);

    return NGX_OK;

failed:

    if (peers->next) {
        ngx_log_debug0(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                       "get least conn peer, backup servers");

        rrp->peers = peers->next;

        n = (rrp->peers->number + (8 * sizeof(uintptr_t) - 1))
                / (8 * sizeof(uintptr_t));

        for (i = 0; i < n; i++) {
             rrp->tried[i] = 0;
        }

        ngx_http_upstream_rr_peers_unlock(peers);

        rc = ngx_http_upstream_get_least_conn_peer(pc, rrp);

        if (rc != NGX_BUSY) {
            return rc;
        }

        ngx_http_upstream_rr_peers_wlock(peers);
    }

    /* all peers failed, mark them as live for quick recovery */

    for (peer = peers->peer; peer; peer = peer->next) {
        peer->fails = 0;
    }

    ngx_http_upstream_rr_peers_unlock(peers);

    pc->name = peers->name;

    return NGX_BUSY;
}


static char *
ngx_http_upstream_least_conn(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_upstream_srv_conf_t  *uscf;

    // 获取所在的upstream{}块
    uscf = ngx_http_conf_get_module_srv_conf(cf, ngx_http_upstream_module);

    if (uscf->peer.init_upstream) {
        ngx_conf_log_error(NGX_LOG_WARN, cf, 0,
                           "load balancing method redefined");
    }

    // 此upstream块的初始化函数
    uscf->peer.init_upstream = ngx_http_upstream_init_least_conn;

    // 指定此upstream块中server指令支持的属性
    uscf->flags = NGX_HTTP_UPSTREAM_CREATE
                  |NGX_HTTP_UPSTREAM_WEIGHT
                  |NGX_HTTP_UPSTREAM_MAX_FAILS
                  |NGX_HTTP_UPSTREAM_FAIL_TIMEOUT
                  |NGX_HTTP_UPSTREAM_DOWN
                  |NGX_HTTP_UPSTREAM_BACKUP;

    return NGX_CONF_OK;
}
