
## nginx的进程模型
### nginx进程结构
    ngx_process_t
### nginx全局进程数组
    ngx_processes
### nginx fork进程的流程
    ngx_spawn_process
### nginx进程间通信
    进程间通信消息 ngx_channel_t
    master worker间通信
    worker worker间通信
        ngx_start_worker_processes
    channel消息处理函数
        ngx_channel_handler
### master与worker之间的交互
    流管道与信号 master与worker之间交互
    信号 master与外部交互

    信号处理函数 ngx_signal_handler
        进程状态全局变量ngx_process
        worker能处理的信号
        master能处理的信号
### master主循环
    ngx_master_process_cycle
### worker主循环
    ngx_worker_process_cycle
    