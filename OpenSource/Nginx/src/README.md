
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
    http/modules/ngx_http_upstream_xxx.c系列文件
