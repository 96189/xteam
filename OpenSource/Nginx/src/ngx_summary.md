
## Q
### 1、nginx的reload机制?
### 2、nginx是如何处理请求的?
### 3、nginx是如何处理网络事件、信号、定时器的?
### 4、nginx架构与高性能的原因?

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

        // 配置项 try_files处理阶段
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
    按阶段挂载content phase handlers
        ngx_http_conf_get_module_main_conf(ngx_conf_t*, ngx_module_t)
        h = ngx_array_push(...)
        *h = ngx_http_<module name>_handler

    按需挂载content handler
        clcf = ngx_http_conf_get_module_loc_conf(ngx_conf_t*, ngx_module_t)
        clcf->handler = ngx_http_<module name>_handler

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
