#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "stack.h"

namespace MYSTL
{
// 顶点状态
typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;

// 边状态
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
    void BFS(int, int&);
    // 深度优先搜索算法
    void DFS(int, int&);
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
    virtual int& dtime(int) = 0;
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
    void bfs(int);
    // 深度优先搜索
    void dfs(int);
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