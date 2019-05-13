
docker部署tars
https://hub.docker.com/r/tarscloud/tars/
1、安装并启动mysql
sudo docker pull mysql:5.6
sudo docker run --name mysql -p 3306:3306 -v /mysql/data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=111111 -d mysql:5.6

2、安装并启动tars
sudo docker pull tarscloud/tars-node:latest
sudo docker run -d -it --name tars --link mysql --env MOUNT_DATA=true --env DBIP=mysql --env DBPort=3306 --env DBUser=root --env DBPassword=111111 -p 3000:3000 -v /tars/data:/data tarscloud/tars

3、查看已安装的服务状态
sudo docker ps

4、其他
查看启动日志
sudo docker container logs 459a818ac476
进入容器内部
sudo docker exec -it a9a8a51f4244 /bin/bash



tars官方文档 https://github.com/TarsCloud/Tars/blob/master/Introduction.md
使用手册 https://tars.tencent.com/base/help/TARS_QCLOUD_HELP_V3.htm

1、tars框架启动流程
/usr/local/app/tars/tars_install.sh
tarsregistry/util/start.sh ;            // 注册中心 服务的注册与发现 Registry.tars
tarsAdminRegistry/util/start.sh;        // 服务管理 启停 发布 AdminReg.tars Patch.tars EndpointF.tars
tarsnode/util/start.sh ;                // Node服务节点 管理业务节点 提供启停 发布 监控等功能 Node.tars
tarsconfig/util/start.sh;               // 配置中心 ConfigF.tars
tarspatch/util/start.sh;                // 发布管理 Patch.tars

tarsnotify/bin/tars_start.sh;           // 异常信息 统计业务上报的各种异常信息(业务上报的信息和框架上报的信息) NotifyF.tars
tarsstat/bin/tars_start.sh;             // 调用统计 统计业务服务上报的各种调用信息 StatF.tars
tarsproperty/bin/tars_start.sh;         // 业务属性 统计业务自定义上报的属性信息 PropertyF.tars
tarslog/bin/tars_start.sh;              // 远程日志 提供服务打日志到远程的功能 LogF.tars
tarsquerystat/bin/tars_start.sh;        // 服务监控
tarsqueryproperty/bin/tars_start.sh;    // 特性监控

2、连接状态

tars-netstat.png图

2.2 tarsregistry 注册中心
启动配置文件 /usr/local/app/tars/tarsregistry/conf/tars.tarsregistry.config.conf
            <QueryAdapter>
                endpoint   = tcp -h 172.17.0.3 -p 17890 -t 10000
                allow      =
                maxconns    = 10000
                threads     = 5
                queuecap    = 10000
                queuetimeout= 4000
                servant     = tars.tarsregistry.QueryObj
            </QueryAdapter>
            <RegistryAdapter>
                endpoint    = tcp -h 172.17.0.3 -p 17891 -t 30000
                allow       =
                maxconns    = 2048
                threads     = 5
                queuecap    = 10000
                queuetimeout= 4000
                servant     = tars.tarsregistry.RegistryObj
            </RegistryAdapter>

17891端口RegistryObj处理服务注册请求 供node访问的对象
17890端口QueryObj处理服务查询请求 供tars的服务获取路由的对象

tarsnode -> tarsregistry:17891
tarsproperty, tarsconfig -> tarsregistry:17890

2.3 tarsAdminRegistry 服务管理
172.17.0.3:12000

本端口无连接 待研究FIX ME

2.4 tarsnode 服务节点
启动配置文件 /usr/local/app/tars/tarsnode/conf/tars.tarsnode.config.conf
                <server>
                        logsize=10M
                        config=tars.tarsconfig.ConfigObj
                        notify=tars.tarsnotify.NotifyObj
                        log=tars.tarslog.LogObj
                        deactivating-timeout=3000
                        app=tars
                        server=tarsnode
                        localip=172.17.0.3
                        local=tcp -h 127.0.0.1 -p 19385 -t 10000
                        basepath=/usr/local/app/tars/tarsnode/data
                        datapath=/usr/local/app/tars/tarsnode/data
                        logpath=/usr/local/app/tars/app_log
                        logLevel=DEBUG
                        <NodeAdapter>
                                endpoint=tcp -h 172.17.0.3 -p 19385 -t 60000
                                allow
                                maxconns=1024
                                threads=5
                                queuecap=10000
                                queuetimeout=4000
                                servant=tars.tarsnode.NodeObj
                        </NodeAdapter>
                        <ServerAdapter>
                                endpoint=tcp -h  172.17.0.3 -p 19386 -t 60000
                                allow
                                maxconns=1024
                                threads=5
                                queuecap=10000
                                queuetimeout=4000
                                servant=tars.tarsnode.ServerObj
                        </ServerAdapter>
                </server>

19385端口NodeObj 
19386端口ServerObj
127.0.0.1:19385端口

tarsnode:19385暂时未发现连接 待研究FIX ME
[tarspatch, tarsstat, tarsconfig, tarsqueryproper, tarslog, tarsnotify, tarsproperty, tarsquerystat] -> tarsnode:19386


2.5 tarsconfig 配置中心
172.17.0.3:10001
127.0.0.1:10001
本端口无连接 待研究FIX ME

2.6 tarspatch 发布管理
172.17.0.3:10000
127.0.0.1:10000
本端口无连接 待研究FIX ME

2.7 tarsnotify 异常信息统计
172.17.0.3:10002
127.0.0.1:10002
本端口无连接 待研究FIX ME

2.8 tarsstat 调用统计
tarsproperty, tarslog, tarsstat -> 172.17.0.3:10003
127.0.0.1:10003

2.9 tarsproperty 业务属性统计
172.17.0.3:10004 与多个服务存在短连接关系
127.0.0.1:10004

2.10 tarslog 远程日志
172.17.0.3:10005 与多个服务存在短连接关系
127.0.0.1:10005

2.11 tarsquerystat
172.17.0.3:10006
127.0.0.1:10006
本端口无连接 待研究FIX ME

2.12 tarsqueryproperty
172.17.0.3:10007
127.0.0.1:10007
本端口无连接 待研究FIX ME

3、添加并发布业务服务
3.1 环境
git clone https://github.com/TarsCloud/TarsCpp.git --recursive
cd TarsCpp
cmake .
make
make install

https://tars.tencent.com/base/help/TARS_quick_start_C++.html
生成服务发布方和服务调用者


服务发布
访问管理界面 ip:3000/index.html#/operation/deploy
应用 按需填写
服务名称 填写二进制名
节点 填写部署ip
OBJ 填写代码中obj名

服务管理/发布管理下选中包进行发布

新服务网络连接状态
以HelloServer为例
HelloServer -> [tarsproperty, tarslog, tarsnode]


客户端直接调用远程服务
客户端间接调用远程服务
建立连接的时机


服务监控与特性监控:
insert2MultiDbs -> insert2Db -> creatTable -> creatTb 按小时生成24张表 分布在数据库tars_property和tars_stat  2*24每天48张表

Q1: 为什么tars_property和tars_stat数据库中的表未生成,导致服务监控和特性监控查询数据时一直错误? 是否是由于线程阻塞?
Q2: 为什么同步日志到远程一直失败?/data/tars/app_log/tars/tarsproperty路径下日志显示远程日志一直失败?远程日志地址是在何时设置的?能否动态关闭远程日志功能?

服务定位器模式 locator

et lt对比
https://www.cnblogs.com/lojunren/p/3856290.html


