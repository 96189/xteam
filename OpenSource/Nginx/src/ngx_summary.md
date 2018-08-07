
## Q
### 1、nginx的reload机制?
### 2、nginx是如何处理请求的?
### 3、nginx是如何处理网络事件、信号、定时器的?
### 4、nginx架构与高性能的原因?

## handler模块
### handler模块的挂载
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
##### <1> 数据结构解析
    配置信息        ngx_http_access_loc_conf_t
    配置指令数组    ngx_http_access_commands[]
    模块上下文      ngx_http_access_module_ctx
    模块            ngx_http_access_module
    模块处理函数    ngx_http_access_handler

#### (2) http_static_module读取磁盘静态文件将文件内容作为输出
    src/http/modules/ngx_http_static_module.c
##### <1> 数据结构解析
    配置信息        ngx_http_core_loc_conf_t
    配置指令数组    -
    模块上下文      ngx_http_static_module_ctx
    模块            ngx_http_static_module
    模块处理函数    ngx_http_static_handler

#### (3) http_log_module记录http请求
    src/http/modules/ngx_http_log_module.c
##### <1> 数据结构解析
    配置信息        ngx_http_log_loc_conf_t
    配置指令数组    ngx_http_log_commands[]
    模块上下文      ngx_http_log_module_ctx
    模块            ngx_http_log_module
    模块处理函数    ngx_http_log_handler
    