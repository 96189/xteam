#ifndef _QUAD_LIST_H_
#define _QUAD_LIST_H_

#include "entry.h"

namespace MYSTL 
{

#define QuadlistNodePosi(T) QuadlistNode<T>* // 跳转表节点位置

// QuadlistNode模板类(四联表节点)
template <typename T>
class QuadlistNode 
{
public:
    T entry;                    // 存储的词条
    QuadlistNodePosi(T) pred;   // 前驱
    QuadlistNodePosi(T) succ;   // 后继
    QuadlistNodePosi(T) above;  // 上邻
    QuadlistNodePosi(T) below;  // 下邻

// 构造函数
    QuadlistNode(T e = T(), 
                 QuadlistNodePosi(T) p = NULL, QuadlistNodePosi(T) s = NULL,
                 QuadlistNodePosi(T) a = NULL, QuadlistNodePosi(T) b = NULL)
        : entry(e), pred(p), succ(s), above(a), below(b)
    {

    }
    // 插入新节点 以当前节点为前驱 以节点b为下邻
    QuadlistNodePosi(T) insertAsSuccAbove(const T& e, QuadlistNodePosi(T) b = NULL)
    {
        QuadlistNodePosi(T) x = new QuadlistNode<T>(e, this, succ, NULL, b);
        succ->pred = x; succ = x; // 设置水平逆向链接
        if (b) b->above = x; // 设置垂直逆向链接
        return x;
    }
};

// Quadlist模板类(四联表)
template <typename T>
class Quadlist 
{
private:
    int _size;  // 规模
    QuadlistNodePosi(T) header;  // 头哨兵
    QuadlistNodePosi(T) trailer; // 尾哨兵
protected:
    void init()    // Quadlist创建时的初始化
    {
        header = new QuadlistNode<T>; // 创建头哨兵节点
        trailer = new QuadlistNode<T>; // 创建尾哨兵节点
        
        header->succ = trailer; // 沿横向连接哨兵
        header->pred = NULL;    
        trailer->pred = header; // 沿横向连接哨兵
        trailer->succ = NULL;

        header->above = trailer->above = NULL; // 纵向的后继置为空
        header->below = trailer->below = NULL; // 纵向的前驱置为空
        _size = 0;
    }
    int clear()    // 清除所有节点
    {
        int oldSize = _size;
        while (0 < _size)
            remove(header->succ);
        return oldSize;
    }
public:
// 构造函数
    Quadlist()
    {
        init();
    }
// 析构函数
    ~Quadlist()
    {
        clear();
        delete header;
        delete trailer;
    }
// 只读访问接口
    Rank size() const 
    {
        return _size;
    }
    bool empty() const 
    {
        return _size <= 0;
    }
    // 首节点位置
    QuadlistNodePosi(T) first() const 
    {
        return header->succ;
    }
    // 末节点位置
    QuadlistNodePosi(T) last() const 
    {
        return trailer->pred;
    }
    // 判断位置p是否对外合法
    bool valid(QuadlistNodePosi(T) p) const
    {
        return p && (trailer != p) && (header != p);
    }
// 可写访问接口
    // 删除合法位置p处的节点 返回被删除的节点的数据
    T remove(QuadlistNodePosi(T) p)
    {
        // 摘除节点
        p->pred->succ = p->succ;
        p->succ->pred = p->pred;
        --_size;
        // 备份词条 释放节点
        T e = p->entry;
        delete p;
        return e;
    }
    // 将e作为p的后继 b的上邻插入
    QuadlistNodePosi(T) insertAsAbove(const T& e, QuadlistNodePosi(T) p, QuadlistNodePosi(T) b = NULL)
    {
        ++_size;
        return p->insertAsSuccAbove(e, b);
    }
// 遍历
    // 遍历各节点 依次实施指定操作(函数指针 只读或局部修改)
    void traverse(void (*)(T&))
    {

    }
    // 遍历各节点 依次实施指定操作(函数对象 可全局性修改节点)
    template <typename VST>
    void traverse(VST& )
    {

    }
}; // Quadlist

};

#endif