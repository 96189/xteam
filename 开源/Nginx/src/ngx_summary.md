## Q
### 1、nginx的reload机制?
### 2、nginx是如何处理请求的?
    http请求格式:请求行 请求头 请求体
    http请求实例:
    http请求处理过程:
        [ ... ]表示函数是事件触发的回调 不是主动调用
    (1)、请求头读取及处理过程:
        ngx_event_process_init
        [ ngx_event_accept ]
            | -> ngx_http_init_connection
        [ ngx_http_ssl_handshake | ngx_http_wait_request_handler ]
            | -> c->recv(c, b->last, size)
            | -> ngx_http_process_request_line
                    | -> ngx_http_read_request_header
                    | -> ngx_http_parse_request_line
                    | -> ngx_http_process_request_headers
                            | -> ngx_http_read_request_header
                            | -> ngx_http_parse_header_line
                            | -> ngx_http_process_request_header
                            | -> ngx_http_process_request
                                    | -> ngx_http_handler
                                    |       | -> ngx_http_core_run_phases
                                    |       |       | -> ph[r->phase_handler].checker(r, &ph[r->phase_handler])
                                    | -> ngx_http_run_posted_requests
                                            | -> write_event_handler <=> ngx_http_core_run_phases
                                                    | -> ph[r->phase_handler].checker(r, &ph[r->phase_handler])

    (2)、请求体读取及处理过程:
    由挂载的handler模块处理 请求体的读取一般发生在nginx的content handler中
    读取请求体的接口 ngx_http_read_client_request_body(ngx_http_request_t *r,ngx_http_client_body_handler_pt post_handler)
    该接口被以下函数调用
        ngx_http_dav_handler(ngx_http_request_t *r)
        ngx_http_fastcgi_handler(ngx_http_request_t *r)
        ngx_http_proxy_handler(ngx_http_request_t *r)
        ngx_http_scgi_handler(ngx_http_request_t *r)
        ngx_http_uwsgi_handler(ngx_http_request_t *r)

    (3)、多阶段请求处理 
        ngx_http_core_run_phases

        ngx_http.c中ngx_http_init_phase_handlers(...)初始化引擎数组

    (4)、发送响应头和响应体
        ngx_http_send_header(ngx_http_request_t *r) ngx_http_top_header_filter全局函数指针执行过滤响应头
        ngx_http_output_filter(ngx_http_request_t *r, ngx_chain_t *in) ngx_http_top_body_filter全局函数指针执行过滤响应体
        响应头和响应体中会有过滤模块进行操作
    (5)、子请求解析原理
    (6)、https支持
    ssl握手ngx_http_ssl_handshake(ngx_event_t *rev) 

### 3、nginx是如何处理网络事件、信号、定时器的?
### 4、nginx架构与高性能的原因?
### 5、父请求与子请求

## 参考: http://tengine.taobao.org/book/
        https://github.com/chronolaw/annotated_nginx

## 一、handler模块
### 1、handler模块的挂载
#### (1) http request处理的11个阶段
    typedef enum {
        // 读取请求内容阶段
        NGX_HTTP_POST_READ_PHASE = 0,

        // server请求地址重写阶段
        NGX_HTTP_SERVER_REWRITE_PHASE,

        // 配置查找阶段
        NGX_HTTP_FIND_CONFIG_PHASE,
        // location请求地址重写阶段
        NGX_HTTP_REWRITE_PHASE,
        // 请求地址重写提交阶段
        NGX_HTTP_POST_REWRITE_PHASE,

        // 访问权限检查准备阶段
        NGX_HTTP_PREACCESS_PHASE,

        // 访问权限检查阶段
        NGX_HTTP_ACCESS_PHASE,
        // 访问权限检查提交阶段
        NGX_HTTP_POST_ACCESS_PHASE,

        // 配置项try_files处理阶段
        NGX_HTTP_TRY_FILES_PHASE,
        // 内容生产阶段
        NGX_HTTP_CONTENT_PHASE,

        // 日志模块处理阶段
        NGX_HTTP_LOG_PHASE
    } ngx_http_phases;

#### (2) 不可挂载的几个特殊阶段
    NGX_HTTP_FIND_CONFIG_PHASE
    NGX_HTTP_POST_REWRITE_PHASE
    NGX_HTTP_POST_ACCESS_PHASE
    NGX_HTTP_TRY_FILES_PHASE

    自定义的模块，大多数是挂载在NGX_HTTP_CONTENT_PHASE阶段的

### 2、自定义hadnler模块步骤
#### (1) 模块配置信息自定义
    命名规则 ngx_http_<module name>_(main | srv | loc)_conf_t 
    代表配置作用域为main server location 与xxx.conf中的配置相对应

#### (2) 配置指令数组自定义
    命名规则 ngx_http_<module name>_commands[]
    变量类型 ngx_command_t

#### (3) 模块上下文结构自定义
    命名规则 ngx_http_<moudle name>_module_ctx
    变量类型 ngx_http_module_t
    该变量的ngx_http_log_init成员提供挂载方法

#### (4) 模块自定义
    命名规则 ngx_http_<module name>_module
    变量类型 ngx_module_t

#### (5) 编写模块处理函数
    命名规则 ngx_http_<module name>_handler
    函数原型 ngx_int_t ngx_http_<module name>_handler(ngx_http_request_t *r)

#### (6) 挂载模块及处理函数
    按阶段挂载 content phase handlers
        ngx_http_conf_get_module_main_conf(ngx_conf_t*, ngx_module_t)
        h = ngx_array_push(...)
        *h = ngx_http_<module name>_handler

    按需挂载 content handler
        clcf = ngx_http_conf_get_module_loc_conf(ngx_conf_t*, ngx_module_t)
        clcf->handler = ngx_http_<module name>_handler
        NGX_HTTP_CONTENT_PHASE阶段检查location是否已经有对应的content handler模块 若存在则执行content handler跳过该阶段挂载的所有content phase handlers.
#### (7) 配置文件中配置

### 3、handler模块实例分析
#### (1) http_access_module对特定客户端的访问控制
    src/http/modules/ngx_http_access_module.c
##### 数据结构解析
    配置信息        ngx_http_access_loc_conf_t
    配置指令数组    ngx_http_access_commands[]
    模块上下文      ngx_http_access_module_ctx
    模块            ngx_http_access_module
    模块处理函数    ngx_http_access_handler

#### (2) http_static_module读取磁盘静态文件将文件内容作为输出
    src/http/modules/ngx_http_static_module.c
##### 数据结构解析
    配置信息        ngx_http_core_loc_conf_t
    配置指令数组    -
    模块上下文      ngx_http_static_module_ctx
    模块            ngx_http_static_module
    模块处理函数    ngx_http_static_handler

#### (3) http_log_module记录http请求
    src/http/modules/ngx_http_log_module.c
##### 数据结构解析
    配置信息        ngx_http_log_loc_conf_t
    配置指令数组    ngx_http_log_commands[]
    模块上下文      ngx_http_log_module_ctx
    模块            ngx_http_log_module
    模块处理函数    ngx_http_log_handler

## 二、filter模块
    源码路径 src/*/ngx_http_xxx_filter_module.c
### 1、过滤模块的目标和实现方式
#### (1) 过滤的目标
    修改http的header和body
#### (2) 过滤的时机
    获取回复给用户的内容之后 向用户发送响应之前 
#### (3) 过滤的内容
    分两阶段:
    先过滤http回复的头部 ngx_http_top_header_filter
    然后过滤http回复的主体 ngx_http_top_body_filter
#### (4) 过滤的执行顺序
    auto/modules执行后生成过滤函数调用顺序 过滤顺序由下自上
    the filter order is important
     ngx_http_write_filter
     ngx_http_header_filter
     ngx_http_chunked_filter
     ngx_http_v2_filter
     ngx_http_range_header_filter
     ngx_http_gzip_filter
     ngx_http_postpone_filter
     ngx_http_ssi_filter
     ngx_http_charset_filter
         ngx_http_xslt_filter
         ngx_http_image_filter
         ngx_http_sub_filter
         ngx_http_addition_filter
         ngx_http_gunzip_filter
         ngx_http_userid_filter
         ngx_http_headers_filter
     ngx_http_copy_filter
     ngx_http_range_body_filter
     ngx_http_not_modified_filter
     ngx_http_slice_filter

#### (5) 过滤的实现
##### <1> 全局变量
    ngx_http.c中全局变量(函数指针)
    ngx_http_output_header_filter_pt  ngx_http_top_header_filter;
    ngx_http_output_body_filter_pt    ngx_http_top_body_filter;
    ngx_http_request_body_filter_pt   ngx_http_top_request_body_filter;
##### <2> 文件作用域的全局变量
    ngx_http_xxx_filter_module.c中static局部全局变量(函数指针)
    static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
    static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;
##### <3> 类似链表的方法执行所有被编译的filter模块
    以ngx_http_addition_filter_module.c为例,这也解释为什么过滤的执行顺序是按照定义的反向执行
    static ngx_int_t
    ngx_http_addition_filter_init(ngx_conf_t *cf)
    {
        // 保存上一个filter模块的处理函数
        ngx_http_next_header_filter = ngx_http_top_header_filter;
        // 被赋值为当前filter模块的处理函数
        ngx_http_top_header_filter = ngx_http_addition_header_filter;

        ngx_http_next_body_filter = ngx_http_top_body_filter;
        ngx_http_top_body_filter = ngx_http_addition_body_filter;

        return NGX_OK;
    }
### 2、过滤模块的细节及优化
#### (1) 实现过滤的底层数据结构
    nginx流式输出模式 -> ngx_chain_t(链表) nginx读到一部分内容就存放到链表中,然后输出
    链表节点的payload -> ngx_buf_t 存储内容 buffer可以表示内存或者文件
#### (2) 过滤的优化
    ngx_chain_t ngx_buf_t采用类似空闲链表的操作.申请和释放都是在池中获取
    ngx_alloc_chain_link  ngx_free_chain
    ngx_chain_get_free_buf  ngx_chain_update_chains
#### (3) 其他
    发出子请求 主请求和子请求响应合并

## 三、upstream模块
    upstream接口回调函数完成请求构造和响应解析等具体的任务
### 1、upstream模块接口定义
#### (1)、ngx_http_upstream_s
    相关回调函数完成业务逻辑 实现后端服务的协议解析
    // 初始化input_filter上下文
    ngx_int_t                      (*input_filter_init)(void *data);
    // 处理后端服务器返回的响应正文
    ngx_int_t                      (*input_filter)(void *data, ssize_t bytes);
    // 生成发送到后端的服务器请求缓冲 初始化upstream用
    ngx_int_t                      (*create_request)(ngx_http_request_t *r);
    // nginx选定新的服务器后 执行重新初始化upstream 再次进行upstream连接
    ngx_int_t                      (*reinit_request)(ngx_http_request_t *r);
    // 处理后端服务器返回的信息头部
    ngx_int_t                      (*process_header)(ngx_http_request_t *r);
    // 客户端放弃请求时被调用  内部不需要关闭后端服务器连接
    void                           (*abort_request)(ngx_http_request_t *r);
    // 正常完成与后端服务器的请求后调用 内部不需要关闭后端服务器连接
    void                           (*finalize_request)(ngx_http_request_t *r, ngx_int_t rc);
    // 重写重定向
    ngx_int_t                      (*rewrite_redirect)(ngx_http_request_t *r, ngx_table_elt_t *h, size_t prefix);
    // 重写cookie
    ngx_int_t                      (*rewrite_cookie)(ngx_http_request_t *r, ngx_table_elt_t *h);
### 2、upstream模块实例解析
    源码路径 http/modules/ngx_http_xxx_module.c
#### (1)、memcached模块
##### 接入memcached模块的步骤
    模块配置信息自定义      typedef struct{...}     ngx_http_memcached_loc_conf_t
    配置指令数组自定义      ngx_command_t           ngx_http_memcached_commands[]
    模块上下文结构自定义    ngx_http_module_t       ngx_http_memcached_module_ctx
    模块自定义             ngx_module_t            ngx_http_memcached_module
    编写模块处理函数        ngx_int_t               ngx_http_memcached_handler(ngx_http_request_t *r)
    挂载模块及处理函数 
        clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module)
        clcf->handler = ngx_http_memcached_handler
    配置文件中配置
##### upstream模块和handler模块的对比
    相同点:模块接入方式一致
    不同点:upstream模块处理函数handler编写存在固定的流程 详情见ngx_http_memcached_handler注释

#### (2)、fastcgi模块
#### (3)、proxy模块

## 四、负载均衡模块
    http://tengine.taobao.org/book/chapter_05.html
### 1、upstream负载均衡模块的配置的内存布局
### 2、负载均衡模块的回调函数体系
    init_upstream -> init_peer -> peer.get -> peer.free
    各函数主要功能:
        init_peer负责建立每个请求使用的server列表
        peer.get负责从server列表中选择某个server
        peer.free负责server释放前的资源释放工作
    不同的负载均衡策略在这个基本框架下设置对应的回调函数完成功能
### 3、upstream整体流程与负载均衡
### 4、负载均衡的接入

## 五、event模块
### epoll ET LT
    Nginx大部分event采用epoll EPOLLET（边沿触发）的方法来触发事件 只有listen端口的读事件是EPOLLLT（水平触发）
### epoll + 非阻塞i/o -> reactor
### nginx对epoll的封装
    modules
### reactor统一定时器事件和信号事件
    定时器底层数据结构 时间复杂度
### 惊群问题与accept锁
    多进程80端口共享 自旋锁 文件锁

## 六、core模块
    nginx进程启动及其初始化

