#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <climits>
#include "stack.h"
#include "queue.h"

namespace MYSTL
{
// 顶点状态(未被发现 已被发现 已被访问) 所有顶点初始状态为UNDISCOVERED
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;

// 边状态(未决的边 树边 跨边 前向边 后向边) 所有边初始状态为UNDETERMINED
// 树边 表示最终作为之成熟保留的边
// 跨边 前向边 后向边 表示最终不保留的边
// 前向边右祖先指向后代 后向边由后代指向祖先
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus;

// 顶点类型 边类型
template <typename Tv, typename Te>
class Graph 
{
private:
    // 所有顶点、边的辅助信息复位
    void reset()
    {
        // 所有顶点
        for (int i = 0; i < n; ++i)
        {
            status(i) = UNDISCOVERED;   // 状态
            dTime(i) = fTime(i) = -1;   // 时间标签
            parent(i) = -1;             // 在遍历树中的父节点
            priority(i) = INT_MAX;      // 优先级数
            // 所有的边
            for (int j = 0; j < n; ++j)
            {
                if (exist(i, j))
                    status(i, j) = UNDETERMINED;
            }
        }
    }
    // 以下5个算法针对连通域
    // 广度优先搜索算法
    // 依赖队列
    // 顶点初始化状态为UNDISCOVERED
    // assert(0 <= v < n)
    void BFS(int v, int& clock)
    {
        Queue<int> q;
        // 选定的某个位置顶点入队
        status(v) = DISCOVERED;
        q.enqueue(v);
        while (!q.empty())
        {
            // 访问当前顶点
            int vertex = q.dequeue();
            dTime(vertex) = ++clock;
            // 枚举vertex位置节点的所有邻居节点neighbor
            for (int neighbor = firstNbr(vertex); 
                 neighbor > -1; 
                 neighbor = nextNbr(vertex, neighbor))
            {
                // 若neighbor未被发现
                if (status(neighbor) == UNDISCOVERED)
                {
                    // 当前neighbor入队
                    status(neighbor) = DISCOVERED;
                    q.enqueue(neighbor);
                    // 边状态置为树边
                    status(vertex, neighbor) = TREE;
                    // 记录树结构
                    parent(neighbor) = vertex;
                }
                // 若neighbor已经被发现 或者 已经访问完毕
                else 
                {
                    // 边状态置为跨边
                    status(vertex, neighbor) = CROSS;
                }
            }
            // 结束当前顶点当问 顶点状态置为visited
            status(vertex) = VISITED;
        }
    }
    // 深度优先搜索算法
    // 依赖递归与回溯
    // assert(0 <= v < n)
    void DFS(int v, int& clock)
    {
        // 开始处理当前节点
        int vertex = v;
        status(vertex) = DISCOVERED;
        dTime(vertex) = ++clock;
        // 枚举当前顶点的所有邻居neighbor
        for (int neighbor = firstNbr(vertex); 
             neighbor > -1; 
             neighbor = nextNbr(vertex, neighbor))
        {
            switch (status(neighbor))
            {
                // 未发现的顶点 支撑树可在此扩展则递归处理
                case UNDISCOVERED:
                    status(vertex, neighbor) = TREE;
                    parent(neighbor) = vertex; 
                    // go
                    DFS(neighbor, clock);
                    // back 回溯到上一个点
                    break;
                // neighbor已被发现但未访问完毕 应属被后台指向的祖先
                case DISCOVERED:
                    status(vertex, neighbor) = BACKWARD;
                    break;
                // neighbor已经访问完毕 视继承关系分为前向边或者跨边
                case VISITED:
                    status(vertex, neighbor) = (dTime(vertex) < dTime(neighbor) ? FORWARD : CROSS);
                    break;
                default:
                    assert(false);
                    break;
            }
        }
        // 结束当前节点处理
        status(vertex) = VISITED;
        fTime(vertex) = ++clock;
    }
    // 基于DFS的双连通分量分解算法
    void BCC(int, int&, Stack<int>&);
    // 基于DFS的拓扑排序算法
    void TSort(int, int&, Stack<Tv>*);
    // 优先级搜索框架
    template <typename PU>
    void PFS(int, PU);
public:
// 顶点
    // 顶点总数
    int n;
    // 插入顶点返回编号
    virtual int insert(Tv const&) = 0;
    // 删除顶点及其关联边 返回该顶点信息
    virtual Tv remove(int) = 0;
    // 顶点v的数据
    virtual Tv& vertex(int) = 0;
    // 顶点v的入度
    virtual int inDegree(int) = 0;
    // 顶点v的出度
    virtual int outDegree(int) = 0;
    // 顶点v的首个邻接点
    virtual int firstNbr(int) = 0;
    // 顶点v的(相对于顶点j的)下一个邻接点
    virtual int nextNbr(int, int) = 0;
    // 顶点v的状态
    virtual VStatus& status(int) = 0;
    // 顶点v的时间标签dTime
    virtual int& dTime(int) = 0;
    // 顶点v的时间标签fTime
    virtual int& fTime(int) = 0;
    // 顶点在遍历树中的父亲
    virtual int& parent(int) = 0;
    // 顶点v在遍历树中的优先级数
    virtual int& priority(int) = 0;
// 边(约定将无向边统一转化为方向互逆的一对有向边 将无向图视作有向图的特例)
    // 边总数
    int e;
    // 边(v,u)是否存在
    virtual bool exist(int, int) = 0;
    // 在顶点v和顶点u之间插入权重为w的边e
    virtual void insert(const Te&, int, int) = 0;
    // 删除顶点v和u之间的边e 返回该边的信息
    virtual Te remove(int, int) = 0;
    // 边(v,u)的状态
    virtual EStatus& status(int, int) = 0;
    // 边(v,u)的数据
    virtual Te& edge(int, int) = 0;
    // 边(v,u)的权重
    virtual int& weight(int, int) = 0;
// 算法
    // 广度优先搜索算法
    // assert(0 <= s < n)
    void bfs(int s)
    {
        assert(0 <= s && s < n);
        reset();
        int clock = 0;
        int v = s;
        // 遍历矩阵
        // 取模判断遍历是否达到矩阵遍历的起点
        do 
        {
            // 当前矩阵中的点还未被发现 则发起一次BFS
            if (status(v) == UNDISCOVERED)
            {
                BFS(v, clock);
            }
        } while(s != (v = (++v % n)));
    }
    // 深度优先搜索
    // assert(0 <= s < n)
    void dfs(int s)
    {
        assert(0 <= s && s < n);
        reset();
        int clock = 0;
        int v = s;
        // 遍历矩阵
        // 取模判断遍历是否达到矩阵遍历的起点
        do 
        {
            // 当前矩阵中的点还未被发现 则发起一次DFS
            if (status(v) == UNDISCOVERED)
            {
                DFS(v, clock);
            }
        } while(s != (v = (++v % n)));
    }
    // 基于dfs的双连通分量分解算法
    void bcc(int);
    // 基于dfs的拓扑排序算法
    Stack<Tv>* tSort(int);
    // 最小支撑树prim算法
    void prim(int);
    // 最短路径dijkstra算法
    void dijkstra(int);
    // 优先级搜索框架
    template <typename PU>
    void pfs(int, PU);
};

};
#endif