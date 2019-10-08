
/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_CYCLE_H_INCLUDED_
#define _NGX_CYCLE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>


#ifndef NGX_CYCLE_POOL_SIZE
#define NGX_CYCLE_POOL_SIZE     NGX_DEFAULT_POOL_SIZE
#endif


#define NGX_DEBUG_POINTS_STOP   1
#define NGX_DEBUG_POINTS_ABORT  2


typedef struct ngx_shm_zone_s  ngx_shm_zone_t;

typedef ngx_int_t (*ngx_shm_zone_init_pt) (ngx_shm_zone_t *zone, void *data);

// nginx共享内存结构体
struct ngx_shm_zone_s {
    // init回调的参数
    void                     *data;
    // 真正操作的共享内存对象
    ngx_shm_t                 shm;
    // 初始化共享内存函数指针
    ngx_shm_zone_init_pt      init;
    // 关联的标记 防止重名 通常是创建共享内存的模块指针
    void                     *tag;
    // reload时是否复用
    ngx_uint_t                noreuse;  /* unsigned  noreuse:1; */
};


// nginx核心数据结构 表示nginx的生命周期
// 启动nginx时的环境参数 配置文件 工作路径等
// 每个进程都有这个结构
struct ngx_cycle_s {
    // 存储所有模块的配置结构体 是个二维数组
    void                  ****conf_ctx;
    // 内存池
    ngx_pool_t               *pool;

    // old_cycle的log对象
    ngx_log_t                *log;
    // 使用error_log指令设置的新log对象
    ngx_log_t                 new_log;

    ngx_uint_t                log_use_stderr;  /* unsigned  log_use_stderr:1; */

    // 文件也当作连接来处理 也是读写操作
    ngx_connection_t        **files;
    // 空闲连接 使用指针串成单向链表 指向第一个空闲连接(头结点)
    ngx_connection_t         *free_connections;
    // 空闲连接的数量
    ngx_uint_t                free_connection_n;

    // 连接复用队列
    ngx_queue_t               reusable_connections_queue;

    // 监听的端口数组 ngx_connection.h
    ngx_array_t               listening;
    // 打开的目录
    ngx_array_t               paths;
    ngx_array_t               config_dump;
    // 打开的文件
    ngx_list_t                open_files;
    // 共享内存
    ngx_list_t                shared_memory;

    // 连接数组的数量
    // 由worker_connections指定 在event模块里设置
    ngx_uint_t                connection_n;
    ngx_uint_t                files_n;

    // 连接池 大小是connection_n
    // 每个连接都有一个读事件和写事件 使用数组序号对应
    // 由ngx_event_core_module的ngx_event_process_init()创建
    ngx_connection_t         *connections;
    // 读事件数组
    // 由ngx_event_core_module的ngx_event_process_init()创建
    ngx_event_t              *read_events;
    // 写事件数组 
    // 由ngx_event_core_module的ngx_event_process_init()创建
    ngx_event_t              *write_events;

    // 保存之前的cycle 如init_cycle
    ngx_cycle_t              *old_cycle;

    // 启动nginx时的配置文件
    ngx_str_t                 conf_file;
    // 启动nginx时的-g参数
    ngx_str_t                 conf_param;
    // #define NGX_CONF_PREFIX  "conf/"
    // 即-c选项指定的配置文件目录
    ngx_str_t                 conf_prefix;
    // #define NGX_PREFIX  "/usr/local/nginx/"
    // 即-p选项指定的工作目录
    ngx_str_t                 prefix;
    // 在linux里直接用共享内存实现锁 此成员无用
    ngx_str_t                 lock_file;
    // 当前主机的hostname ngx_init_cycle()里初始化 全小写
    ngx_str_t                 hostname;
};


// ngx_core_module的配置结构体 在nginx.c里设置
typedef struct {
    // 守护进程是否启用
    ngx_flag_t daemon;
    // master/worker进程机制是否启用
    ngx_flag_t master;

    //调用time_update的时间分辨率 毫秒 在event模块里使用
    ngx_msec_t timer_resolution;

    // worker进程的数量
    ngx_int_t worker_processes;
    // 是否使用debug point
    ngx_int_t debug_points;

    // 可打开的最大文件数量 超过则报ENOFILE错误
    ngx_int_t rlimit_nofile;
    // coredump文件大小
    off_t rlimit_core;

    int priority;

    ngx_uint_t cpu_affinity_n;
    uint64_t *cpu_affinity;

    // nginx运行使用的用户名 默认是nobody
    // objs/ngx_auto_config.h:#define NGX_USER  "nobody"
    char *username;
    ngx_uid_t user;
    ngx_gid_t group;

    // core dump的目录
    ngx_str_t working_directory;
    // 用于实现共享锁 linux下无意义
    ngx_str_t lock_file;

    // master进程的pid文件名
    ngx_str_t pid;
    // new binary时老nginx的pid文件名
    ngx_str_t oldpid;

    ngx_array_t env;
    char **environment;
} ngx_core_conf_t;


#define ngx_is_init_cycle(cycle)  (cycle->conf_ctx == NULL)


ngx_cycle_t *ngx_init_cycle(ngx_cycle_t *old_cycle);
ngx_int_t ngx_create_pidfile(ngx_str_t *name, ngx_log_t *log);
void ngx_delete_pidfile(ngx_cycle_t *cycle);
ngx_int_t ngx_signal_process(ngx_cycle_t *cycle, char *sig);
void ngx_reopen_files(ngx_cycle_t *cycle, ngx_uid_t user);
char **ngx_set_environment(ngx_cycle_t *cycle, ngx_uint_t *last);
ngx_pid_t ngx_exec_new_binary(ngx_cycle_t *cycle, char *const *argv);
uint64_t ngx_get_cpu_affinity(ngx_uint_t n);
ngx_shm_zone_t *ngx_shared_memory_add(ngx_conf_t *cf, ngx_str_t *name,
    size_t size, void *tag);


extern volatile ngx_cycle_t  *ngx_cycle;
extern ngx_array_t            ngx_old_cycles;
extern ngx_module_t           ngx_core_module;
extern ngx_uint_t             ngx_test_config;
extern ngx_uint_t             ngx_dump_config;
extern ngx_uint_t             ngx_quiet_mode;


#endif /* _NGX_CYCLE_H_INCLUDED_ */
