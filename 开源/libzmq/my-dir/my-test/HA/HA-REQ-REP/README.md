
# 请求响应模式的可靠性方案
## Client-Side Reliability ##
### Lazy Pirate Pattern 
    n : 1模型(n个REP客户端访问1个REQ服务器)
* 客户端轮询REQ套接字 真实发生应答才接受
* 客户端在超时时间内没有收到应答 尝试重发
* 重试多次后依然失败 则放弃

    依赖服务器的稳定性 服务器永久故障 则服务永远不可用

## Basic Reliable Queuing ##
### Simple Pirate Pattern
    n : 1 : n模型(n个REP客户端 1个分发queue n个后端worker)
    后端worker是无状态的 后端worker是可以任意上线下线的 客户端不必了解这些 后端worker挂掉之后 会有另一个工作者接替(queue的lru算法)
    适用于n:n的结构  缺点也很明显 中心分发queue节点的单点故障 queue重启后无法识别后端worker 后端空闲的worker挂掉queue也无法感知

## Robust Reliable Queuing ##
### Paranoid Pirate Pattern
    与Basic Reliable Queuing(Simple Pirate Pattern)模式相比 本模式并没有解决queue的单点故障问题 
    通过queue与后端workers之间的双向心跳 解决queue重启后无法感知后端worker以及后端空闲的worker挂掉queue无法感知的两个问题


## High-Availability Pair ##
### Binary Star Pattern
    primary-backup主备模式 客户端配置服务器主备地址 主备机互相探测心跳 主机挂掉 自动故障转移 备机提升为主机


# 协议化 服务化
## Service-Oriented Reliable Queuing ##
### Majordomo Pattern


# 异步化


