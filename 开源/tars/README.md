
# 腾讯微服务架构tars #
https://github.com/TarsCloud/Tars

## 服务路由 ##
### 透明化路由 ###
* 基于服务注册中心的订阅发布
* 消费者缓存服务提供者地址
* 负载均衡
        随机
        轮询
        服务调用时延
        一致性哈希
## 服务注册中心 ##
### 如何做到HA? ###
        支持数据持久化 支持集群 对等集群防止单点故障
### 如何保证数据一致性？###
        如何在数据变更时主动推送?订阅发布机制 服务健康状态监测

### 基于zookeeper的服务注册中心设计 ###
        统一命名服务 状态同步服务 集群管理 分布式应用统一配置
        服务健康状态监测
        对等集群防止单点故障(持久化存储的事务日志和快照 zab协议保证数据一致性)


