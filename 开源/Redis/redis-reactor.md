# 网络库服务器客户端
## 一、事件调度和执行规则

## 二、创建客户端并挂载到server的clients链表尾部的时机
    acceptTcpHandler -> acceptCommonHandler -> createClient -> listAddNodeTail
## 三、redis命令的执行
    readQueryFromClient -> processInputBuffer -> processCommand -> call

# other
## 全局唯一id生成方法
    事件定时器id
    客户端id
    服务id
    慢查询标识符
    Redis通过异步IO和pipelining等机制来实现高速的并发访问
    HyperLogLog algorithm -> MurmurHash64A