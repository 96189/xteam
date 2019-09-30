#ifndef _MATRIX_GRAPH_H_
#define _MATRIX_GRAPH_H_

#include "graph.h"

namespace MYSTL 
{

// 顶点对象
template <typename Tv>
class Vertex 
{
public:
    Tv data;        // 数据
    int inDegree;   // 入度
    int outDegree;  // 出度
    VStatus status; // 状态 UNDISCOVERED, DISCOVERED, VISITED
    int dTime;      // 时间标签 顶点被开始处理的时间
    int fTime;      // 时间标签 顶点被结束处理的时间
    int parent;     // 当前顶点在支撑树中的父节点
    int priority;   // 当前顶点的优先级
// 顶点构造
    Vertex(const Tv& d = Tv())
        : data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
          dTime(-1), fTime(-1), parent(-1), priority(INT_MAX)
    {

    }
};

// 边对象
template <typename Te>
class Edge 
{
public:
    Te data;        // 数据
    int weight;     // 权重
    EStatus status; // 状态 UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD
// 边构造
    Edge(const Te& d, int w)
        : data(d), weight(w), status(UNDETERMINED)
    {

    }
};

// 基于向量 以邻接矩阵实现的图
template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>
{
private:
    Vector<Vertex<Tv> > V;          // 顶点集(向量表示)
    Vector<Vector<Edge<Te>*> > E;   // 边集(矩阵表示))
public:
// 构造
    GraphMatrix()
    {
        this->n = this->e = 0;
    }
// 析构
    ~GraphMatrix()
    {
        // 释放所有动态创建的边记录
        for (int i = 0; i < this->n; ++i)
        {
            for (int j = 0; j < this->n; ++j)
            {
                // 判断[i][j]存在边删除
                delete E[i][j];
            }
        }
    }
// 顶点的基本操作
    // 取i位置顶点的数据
    virtual Tv& vertex(int i)
    {
        return V[i].data;
    }
    // 取i位置顶点的入度
    virtual int inDegree(int i)
    {
        return V[i].inDegree;
    }
    // 取i位置顶点的出度
    virtual int outDegree(int i)
    {
        return V[i].outDegree;
    }
    // 取i位置顶点的第一个邻居
    // 此处的第一个邻居对应于矩阵一行的最后一个元素
    virtual int firstNbr(int i)
    {
        return nextNbr(i, this->n);
    }
    // 取i位置顶点相对于j位置顶点的下一个邻接顶点
    virtual int nextNbr(int i, int j)
    {
        // 逆向线性试探
        while (-1 < j && (!exist(i, --j)));
        return j;
    }
    // 取i位置顶点的状态
    virtual VStatus& status(int i)
    {
        return V[i].status;
    }
    // 取i位置顶点开始处理的时间
    virtual int& dTime(int i)
    {
        return V[i].dTime;
    }
    // 取i位置顶点结束处理的时间
    virtual int& fTime(int i)
    {
        return V[i].fTime;
    }
    // 取i位置顶点的直接parent
    virtual int& parent(int i)
    {
        return V[i].parent;
    }
    // 取i位置定的的优先级
    virtual int& priority(int i)
    {
        return V[i].priority;
    }
// 顶点的动态操作
    // 插入顶点 返回编号
    virtual int insert(const Tv& vertex)
    {
        // 各顶点预留一条潜在的关联边
        for (int j = 0; j < this->n; ++j)
        {
            E[j].insert(NULL);
        }
        this->n++;
        // 创建新顶点对应的边向量
        E.insert(Vector<Edge<Te>*>(this->n, this->n, (Edge<Te>*)NULL));
        // 顶点向量增加一个顶点
        return V.insert(Vertex<Tv>(vertex));
    }
    // 删除第i个顶点及其关联的边
    virtual Tv remove(int i)
    {
        // 所有出边 逐条删除
        for (int j = 0; j < this->n; ++j)
        {
            if (exist(i, j))
            {
                delete E[i][j];
                V[j].inDegree--;
            }
        }
        // 删除第i行
        E.remove(i);
        this->n--;
        // 所有出边 逐条删除
        for (int j = 0; j < this->n; ++j)
        {
            if (exist(j, i))
            {
                delete E[j].remove(i);
                V[j].outDegree--;
            }
        }
        // 删除顶点i
        Tv vBak = vertex(i);
        V.remove(i);
        return vBak;
    }
// 边的确认操作
    // 边(i, j)是否存在
    virtual bool exist(int i, int j)
    {
        return (0 <= i) && (i < this->n) && (0 <= j) && (j < this->n) && E[i][j] != NULL;
    }
// 边的基本操作
    // 取(i,j)顶点之间的边的状态
    virtual EStatus& status(int i, int j)
    {
        return E[i][j]->status;
    }
    // 取(i,j)顶点之间的边的数据
    virtual Te& edge(int i, int j)
    {
        return E[i][j]->data;
    }
    // 取(i,j)顶点之间的边的权重
    virtual int& weight(int i, int j)
    {
        return E[i][j]->weight;
    }
// 边的动态操作
    // 插入权重为w的边e = (i,j)
    virtual void insert(const Te& edge, int w, int i, int j)
    {
        // 确保该边尚不存在
        if (exist(i, j)) return;
        // 创建边
        E[i][j] = new Edge<Te>(edge, w);
        this->e++;
        V[i].outDegree++;
        V[j].inDegree++;
    }
    // 删除顶点i和j之间的边
    virtual Te remove(int i, int j)
    {
        Te eBak = edge(i, j);
        delete E[i][j];
        E[i][j] = NULL;
        this->e--;
        V[i].outDegree--;
        V[j].inDegree--;
        return eBak;
    }
};

};

#endif