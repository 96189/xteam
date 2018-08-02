
## nginx中的负载均衡算法
### 默认负载均衡算法
    平滑的加权轮询算法smooth weighted round-robin balancing
    https://blog.csdn.net/zhangskd/article/details/50194069

    weight              // 配置文件中的权重,代码中固定不变
    current_weight      // 后端服务器当前权重,初始为0,后续同态调整
    effective_weight    // 后端服务器有效权重,初始为weight,错误时降低此值,后续有新请求时逐步恢复至weight

    假设后端有3台服务器分别编号为A B C，配置的weight分别为{ w(A)=4, w(B)=2, w(C)=1 }
    普通的加权轮询算法结果: A A A B A B C
    平滑的加权轮询算法结果: A B A C A B A

    平滑的加权轮询算法产生的服务器分布序列更均匀

#### 实例分析
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

##### 初始化:
        A: ew(A) = w(A) = 4; cw(A) = 0
        B: ew(B) = w(B) = 2; cw(B) = 0
        C: ew(C) = w(C) = 1; cw(C) = 0
    
##### 循环选择:   
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
    
##### effective_weight的调整:
    // 发生错误时降低有效权值
    ngx_http_upstream_free_round_robin_peer
        -> if (peer->max_fails) { peer->effective_weight -= peer->weight / peer->max_fails; }
    // 新请求到达时逐步恢复有效权值到weight
    ngx_http_upstream_get_peer
        -> if (peer->effective_weight < peer->weight) { peer->effective_weight++; }

### nginx可配置的负载均衡算法
    源码位于http/modules/ngx_http_upstream_xxx.c系列文件

#### IP哈希
    https://blog.csdn.net/zhangskd/article/details/50208527

        nginx中配置ip hash后,若对一次请求选择后端服务器时选择次数超过20次或者后端只有一台服务器则会采用smooth weighted 
    round-robin balancing

##### 算法步骤:
    (1) 计算ip的hash值
    // 89质数
    hash = iphp->hash; 
    // 同一网段认为是相同ip 对于ipv4取点分十进制ip地址的前三个数字
    int n = { ipv4 => 3, ipv6 => 16 };
    for (i = 0; i < n; i++) 
    {
        // 113 6271都为质数 使结果更随机
        hash = (hash * 113 + iphp->addr[i]) % 6271;
    }

    (2) 根据hash值得到有效的权重
    w = hash % iphp->rrp.peers->total_weight;
    
    (3) 选择后端服务器
    peer = iphp->rrp.peers->peer;
    while (w >= peer->weight) 
    {
        w -= peer->weight;
        peer = peer->next;
    } 

##### 选择的确定性和不变性
        对于某一确定的ip地址(14.116.139.xx) 计算得到的hash值总是确定不变的 由于后端机器列表的总权重不变且
    每一台机器的权重不变 根据以上算式总能得到唯一不变的服务器

#### 一致性哈希算法Consistent hashing
    https://blog.csdn.net/cywosp/article/details/23397179
    https://blog.csdn.net/zhangskd/article/details/50256111
    
    一致性哈希解决的问题
        解决服务器的增减带来的缓存失效问题

    关键词:
        环(%虚拟的0~2^32-1空间)
        哈希算法(hash => crc32)
        映射数据到环(hash(key))
        映射机器节点到环(hash(...))
        顺时针(遍历数组下标取mod))
        虚拟节点(环上的实体)

    一致性哈希算法步骤

    nginx对一致性哈希的实现

    nginx一致性哈希处理服务器增减的实现

### 一些问题
    1、后端集群中有服务器宕机,各负载均衡算法如何处理?

    2、各算法的优劣势及应用场景?
    