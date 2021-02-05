
## nginx基本配置
    
### 网格相关配置
    proxy_connect_timeout 与下游的连接超时时间(tcp握手时间)
    proxy_read_timeout 从下游读取响应的超时时间(tcp established后下游响应以及处理的时间)
    proxy_send_timeout 往下游发送请求的超时时间

### location匹配

    location 可选的修饰符 uri { ... }

    修饰符:
        = 完全匹配
        ~ 匹配uri大小写敏感
        ~* 匹配uri忽略大小写
        ^~ 最佳匹配，前半部分与uri匹配即可

## nginx重试机制
[官方文档](http://nginx.org/en/docs/http/ngx_http_proxy_module.html)

* 1、为什么要重试？

    容错和高可用，下游服务中有一个实例访问出问题，自动切换到下一个实例。

* 2、重试配置

    proxy_next_upstream_tries 重试的最大次数(包含当前请求，不配置默认值为0)

    proxy_next_upstream_timeout 重试的最大超时时间
    
    proxy_next_upstream 什么情况下重试或者是否开启此功能(默认为error timeout)

* 3、重试的问题与取舍

    重试是将后端服务器列表遍历一遍，重试条件需要正确配置，404不应该重试。考虑写场景，是否应配置重试，是否导致重复写入问题等。

## 负载均衡机制
    不配置时默认使用rr轮询模式

## 协议相关
* tcp

    tcp_nopush

    tcp_nodelay

    keepalive_timeout
