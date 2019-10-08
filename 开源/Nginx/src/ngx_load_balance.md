
## nginx中的负载均衡算法
### 一、默认负载均衡算法
    平滑的加权轮询算法smooth weighted round-robin balancing
    https://blog.csdn.net/zhangskd/article/details/50194069

    weight              // 配置文件中的权重,代码中固定不变
    current_weight      // 后端服务器当前权重,初始为0,后续同态调整
    effective_weight    // 后端服务器有效权重,初始为weight,错误时降低此值,后续有新请求时逐步恢复至weight

    假设后端有3台服务器分别编号为A B C，配置的weight分别为{ w(A)=4, w(B)=2, w(C)=1 }
    普通的加权轮询算法结果: A A A B A B C
    平滑的加权轮询算法结果: A B A C A B A

    平滑的加权轮询算法产生的服务器分布序列更均匀

#### 1、实例分析
    以下表中对应7次请求的分配及其计算过程

    | seq | current_weight befor select | select peer | current_weight after select | 
    | :-: | :-------------------------: | :---------: | :-------------------------: |
    |  0  |        { 0, 0, 0 }          |      -      |        { 0, 0, 0 }          |
    |  1  |        { 4, 2, 1 }          |      A      |        { -3, 2, 1 }         |
    |  2  |        { 1, 4, 2 }          |      B      |        { 1, -3, 2 }         |
    |  3  |        { 5, -1, 3 }         |      A      |        { -2, -1, 3 }        |
    |  4  |        { 2, 1, 4 }          |      C      |        { 2, 1, -3 }         |
    |  5  |        { 6, 3, -2 }         |      A      |        { -1, 3, -2 }        |
    |  6  |        { 3, 5, -1 }         |      B      |        { 3, -2, -1 }        |
    |  7  |        { 7, 0, 0 }          |      A      |        { 0, 0, 0 }          |

##### (1)、初始化:
        A: ew(A) = w(A) = 4; cw(A) = 0
        B: ew(B) = w(B) = 2; cw(B) = 0
        C: ew(C) = w(C) = 1; cw(C) = 0
    
##### (2)、循环选择:   
        ngx_http_upstream_get_peer
        n = len({A, B, C})
        while (n--)
        {
            // current_weight befor select
            peer->current_weight += peer->effective_weight
            total_ew += peer->effective_weight
            // select peer
            best = CompareSaveMaxCW(current_weight)
        }
        // current_weight after select
        best->current_weight -= total_ew 
    
##### (3)、effective_weight的调整:
    // 发生错误时降低有效权值
    ngx_http_upstream_free_round_robin_peer
        -> if (peer->max_fails) { peer->effective_weight -= peer->weight / peer->max_fails; }
    // 新请求到达时逐步恢复有效权值到weight
    ngx_http_upstream_get_peer
        -> if (peer->effective_weight < peer->weight) { peer->effective_weight++; }

### 二、nginx可配置的负载均衡算法
    源码位于http/modules/ngx_http_upstream_xxx.c系列文件

#### 1、IP哈希
    https://blog.csdn.net/zhangskd/article/details/50208527

        nginx中配置ip hash后,若对一次请求选择后端服务器时选择次数超过20次或者后端只有一台服务器则会采用smooth weighted 
    round-robin balancing

##### (1)、算法步骤:
    <1> 计算ip的hash值
    // 89质数
    hash = iphp->hash; 
    // 同一网段认为是相同ip 对于ipv4取点分十进制ip地址的前三个数字
    int n = { ipv4 => 3, ipv6 => 16 };
    for (i = 0; i < n; i++) 
    {
        // 113 6271都为质数 使结果更随机
        hash = (hash * 113 + iphp->addr[i]) % 6271;
    }

    <2> 根据hash值得到有效的权重
    w = hash % iphp->rrp.peers->total_weight;
    
    <3> 选择后端服务器
    peer = iphp->rrp.peers->peer;
    while (w >= peer->weight) 
    {
        w -= peer->weight;
        peer = peer->next;
    } 

##### (2)、选择的确定性和不变性
    对于某一确定的ip地址(14.116.139.xx) 计算得到的hash值总是确定不变的 由于后端机器列表的总权重不变且
每一台机器的权重不变 根据以上算式总能得到唯一不变的服务器

#### 2、一致性哈希算法Consistent hashing
    https://blog.csdn.net/cywosp/article/details/23397179
    https://blog.csdn.net/zhangskd/article/details/50256111
    
##### (1)、一致性哈希解决的问题
    解决服务器的增减带来的缓存失效问题

##### (2)、关键词:
    环(%虚拟的0~2^32-1空间)
    哈希算法(hash => crc32)
    映射数据到环(hash(key))
    映射机器节点到环(通过虚拟节点间接映射)
    顺时针(遍历数组下标取mod))
    虚拟节点(环上的实体)

##### (3)、nginx中一致性哈希的实现
###### 建环 -> 建虚拟节点 -> 映射虚拟节点到环上
    ngx_http_upstream_init_chash
    创建虚拟节点数组total_weight * 160,遍历后端服务器节点对当前peer->weight*160个虚拟节点初始化,crc32算法计算hash,关联
虚拟节点的server字段到peer->server字段.对虚拟节点数组按照hash排序,对有序数组按照hash去重.

###### 映射请求到环上 -> 选取虚拟节点
    ngx_http_upstream_init_chash_peer
    计算请求的哈希 hash = ngx_crc32_long(hp->key.data, hp->key.len)
    根据请求的hash值顺时针方向找最近(大于等于hash)的虚拟节点 hp->hash = ngx_http_upstream_find_chash_point
(hcf->points, hash),hp->hash记录此虚拟节点在数组中的索引

###### 虚拟节点与真实节点的映射关系
    server字段相同,已知虚拟节点的server字段,可在后端服务器真实节点中遍历

###### 选取真实的节点
    ngx_http_upstream_get_chash_peer
    取虚拟节点的server字段server = point[hp->hash % points->number].server
    循环遍历后端服务器列表server字段相同的选取权重最大的节点

##### nginx一致性哈希如何处理服务器的增加的情况,代码中如何体现?

#### 3、最少连接算法least conn
##### (1)、算法步骤:
    <1> 遍历后端集群,选择peer->conns * peer->weight最小的节点,若不存在重复的最小peer->conns * peer->weight则算法结
束,否则执行<2>
    <2> 遍历后端集群中peer->conns * peer->weight相等且为最小值的服务器节点,对这些服务器节点按照加权轮询算法选择权重最
大的服务器节点

### 三、一些问题
#### 1、后端集群中有服务器宕机,各负载均衡算法如何处理?
    smooth weighted round-robin balancing:
        后端服务器节点宕机,对一次请求,选择节点来说,循环中选择即使当前宕机节点权重最大也会跳过当前宕机节点选择权重第二大的
    节点.
    
    IP HASH:
        后端服务器节点宕机,对一次请求,选择节点来说,若哈希命中宕机节点则循环会始终命中到宕机节点,尝试20次,若该节点一直处于
    宕机状态则采用平滑的加权轮询算法选择机器.
    
    Consistent hashing:
        在后端有真实节点宕机时,对一次请求,选择节点来说会命中到宕机节点,但循环中过滤掉宕机几点,hash++因此会选择
    下一个节点,不会造成服务不可用的情况.
#### 2、各算法的优劣势及应用场景?
    smooth weighted round-robin balancing:
        优:根据机器配置可均衡将负载分配到各机器,完全依靠后端实际情况选择.
        缺:同一客户端的多次请求可能会被分配到不同的后端服务器处理,无法满足做会话保持的应用需求.
        适用场景:每个请求所占用的后端时间差不多
    
    IP HASH
        优:可做session保持,同一个ip始终由一台机器处理
        缺:可能导致分配不均匀
    
    Consistent hashing
        优:解决动态增加机器问题
    
    least conn
        适用场景: 某些请求占用的时间很长,平滑的加权轮询算法若单纯按照权值分配会造成较大延迟,这种场景下将请求转发给连接数
    较少的后端更好