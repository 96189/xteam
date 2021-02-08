
## [nginx基本配置](https://www.nginx.com/resources/wiki/start/topics/examples/full/)

[配置指令wiki](http://nginx.org/en/docs/ngx_core_module.html#worker_processes)

```
nginx.conf
user       www www;  ## Default: nobody
# Default: 1 
# 建议配置为CPU核数，配置为auto时，尝试检查CPU核数
worker_processes  4;  
error_log  logs/error.log;
pid        logs/nginx.pid;
# 系统最多打开的文件数
worker_rlimit_nofile 65535;

events {
    # 单个进程最大连接数
    # Default: 1024
    # 个数上限 
    #   系统内存 > worker_processes * worker_connections * 每个连接占用的内存
    #   worker_rlimit_nofile > worker_connections
    worker_connections  40000;  
    # Default: off
    # 打开时一次接受所有请求，关闭时一次接受1个请求
    multi_accept on;
    # 支持的事件模型 kqueue rtsig epoll /dev/poll select poll
    use epoll;
}

http {
    include    mime.types;
    include    proxy.conf;
    include    fastcgi.conf;
    index    index.html index.htm index.php;

    # request header size 默认1k
    client_header_buffer_size 1k;
    # 分块传输 默认on
    chunked_transfer_encoding on;

    default_type application/octet-stream;

    # 与fastcgi建立连接的超时时间(不能超过75s) 默认60s
    fastcgi_connect_timeout 60;
    # 请求传输到fastcgi服务器的超时 默认60s
    # 超时表示2次写之间的超时
    fastcgi_send_timeout 60;
    # 从fastcgi读取响应的超时 默认60s
    # 超时表示2次读取之间的操作
    fastcgi_read_timeout 60;
    # 用于读取fastcgi响应的缓冲区大小
    # size大小取决于平台，通常是memory page 默认4k or 8k
    fastcgi_buffer_size 4k|8k;
    # 缓冲区数量	
    fastcgi_buffers 8 4k|8k;
    # TODO
    fastcgi_busy_buffers_size 8k|16k;
    fastcgi_temp_file_write_size 8k|16k;
    fastcgi_intercept_errors off;

    # 是否开启proxy忽略客户端中断 默认off
    # 如果此项设置为on开启，则服务器会忽略客户端中断，一直等着代理服务执行返回。并且如果执行没有发生错误，记录的日志是200日志。如果超时则会记录504。如果设置为off，则客户端中断后服务器端nginx立即记录499日志
    proxy_ignore_client_abort off;
    log_format   main '$remote_addr - $remote_user [$time_local]  $status '
        '"$request" $body_bytes_sent "$http_referer" '
        '"$http_user_agent" "$http_x_forwarded_for"';
    access_log   logs/access.log  main;

    # 定义可信地址
    set_real_ip_from 10.0.0.0/8;
    # request header field 用于替换客户端地址
    real_ip_header CLIENTIP;
    # 设置用于代理的HTTP协议版本
    proxy_http_version 1.1;

    # TODO
    sendfile     on;
    # TCP_NOPUSH选项 默认on
    tcp_nopush   on;
    # TCP_NODELAY选项 默认on
    tcp_nodelay on;
    # tcp连接keep alive保活定时器超时时间
    # 默认75s
    keepalive_timeout 75s;
    server_names_hash_bucket_size 128; # this seems to be required for some vhosts



    server { # php/fastcgi
        listen       80;
        server_name  domain1.com www.domain1.com;
        access_log   logs/domain1.access.log  main;
        root         html;
        # 重试次数
        proxy_next_upstream_tries 2;

        location ~ \.php$ {
            fastcgi_pass   127.0.0.1:1025;
        }
    }

    server { # simple reverse-proxy
        listen       80;
        server_name  domain2.com www.domain2.com;
        access_log   logs/domain2.access.log  main;

        # serve static files
        location ~ ^/(images|javascript|js|css|flash|media|static)/  {
            root    /var/www/virtual/big.server.com/htdocs;
            expires 30d;
        }

        # pass requests for dynamic content to rails/turbogears/zope, et al
        location / {
            proxy_pass      http://127.0.0.1:8080;
        }
    }

    upstream big_server_com {
        server 127.0.0.3:8000 weight=5;
        server 127.0.0.3:8001 weight=5;
        server 192.168.0.1:8000;
        server 192.168.0.1:8001;
    }

    server { # simple load balancing
        listen          80;
        server_name     big.server.com;
        access_log      logs/big.server.access.log main;

        location / {
            proxy_pass      http://big_server_com;
        }
    }
}
```
    
```
proxy.conf
proxy_redirect          off;
proxy_set_header        Host            $host;
proxy_set_header        X-Real-IP       $remote_addr;
proxy_set_header        X-Forwarded-For $proxy_add_x_forwarded_for;
client_max_body_size    10m;
client_body_buffer_size 128k;
proxy_connect_timeout   90;
proxy_send_timeout      90;
proxy_read_timeout      90;
proxy_buffers           32 4k;
```

```
fastcgi.conf
fastcgi_param  SCRIPT_FILENAME    $document_root$fastcgi_script_name;
fastcgi_param  QUERY_STRING       $query_string;
fastcgi_param  REQUEST_METHOD     $request_method;
fastcgi_param  CONTENT_TYPE       $content_type;
fastcgi_param  CONTENT_LENGTH     $content_length;
fastcgi_param  SCRIPT_NAME        $fastcgi_script_name;
fastcgi_param  REQUEST_URI        $request_uri;
fastcgi_param  DOCUMENT_URI       $document_uri;
fastcgi_param  DOCUMENT_ROOT      $document_root;
fastcgi_param  SERVER_PROTOCOL    $server_protocol;
fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
fastcgi_param  SERVER_SOFTWARE    nginx/$nginx_version;
fastcgi_param  REMOTE_ADDR        $remote_addr;
fastcgi_param  REMOTE_PORT        $remote_port;
fastcgi_param  SERVER_ADDR        $server_addr;
fastcgi_param  SERVER_PORT        $server_port;
fastcgi_param  SERVER_NAME        $server_name;

fastcgi_index  index.php;

fastcgi_param  REDIRECT_STATUS    200;
```

```
types {
  text/html                             html htm shtml;
  text/css                              css;
  text/xml                              xml rss;
  image/gif                             gif;
  image/jpeg                            jpeg jpg;
  application/x-javascript              js;
  text/plain                            txt;
  text/x-component                      htc;
  text/mathml                           mml;
  image/png                             png;
  image/x-icon                          ico;
  image/x-jng                           jng;
  image/vnd.wap.wbmp                    wbmp;
  application/java-archive              jar war ear;
  application/mac-binhex40              hqx;
  application/pdf                       pdf;
  application/x-cocoa                   cco;
  application/x-java-archive-diff       jardiff;
  application/x-java-jnlp-file          jnlp;
  application/x-makeself                run;
  application/x-perl                    pl pm;
  application/x-pilot                   prc pdb;
  application/x-rar-compressed          rar;
  application/x-redhat-package-manager  rpm;
  application/x-sea                     sea;
  application/x-shockwave-flash         swf;
  application/x-stuffit                 sit;
  application/x-tcl                     tcl tk;
  application/x-x509-ca-cert            der pem crt;
  application/x-xpinstall               xpi;
  application/zip                       zip;
  application/octet-stream              deb;
  application/octet-stream              bin exe dll;
  application/octet-stream              dmg;
  application/octet-stream              eot;
  application/octet-stream              iso img;
  application/octet-stream              msi msp msm;
  audio/mpeg                            mp3;
  audio/x-realaudio                     ra;
  video/mpeg                            mpeg mpg;
  video/quicktime                       mov;
  video/x-flv                           flv;
  video/x-msvideo                       avi;
  video/x-ms-wmv                        wmv;
  video/x-ms-asf                        asx asf;
  video/x-mng                           mng;
}
```
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

    proxy_next_upstream_tries 重试的最大次数(包含当前请求，不配置默认值为0，表示不限制次数)

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
