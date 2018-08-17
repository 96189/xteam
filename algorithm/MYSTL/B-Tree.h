#ifndef _B_SUB_TREE_H_
#define _B_SUB_TREE_H_

#include "bNode.h"
namespace MYSTL 
{

// B-树模板类
template <typename T>
class BTree 
{
protected:
    int _size;              // 存放的关键码的总数
    int _order;             // B-树的阶次 至少为3
    BTNodePosi(T) _root;    // 根节点
    BTNodePosi(T) _hot;     // search()最后访问的非空节点位置
    // 因插入而上溢之后的分裂处理
    void solveOverflow(BTNodePosi(T) t)
    {

    }
    // 因删除而下溢之后的合并处理
    void solveUnderflow(BTNodePosi(T) t)
    {

    }
    // 释放x指向的树
    void release(BTNodePosi(T) x)
    {
        
    }
public:
    // 构造函数 默认为最低的3阶
    BTree(int order = 3) 
        : _order(order), _size(0)
    {
        _root = new BTNode<T>();
    }
    // 析构函数 释放所有节点
    ~BTree()
    {
        if (_root) release(_root);
    }
    // 阶次
    int order() const 
    {
        return _order;
    }
    // 规模
    int size() const 
    {
        return _size;
    }
    // 树根
    BTNodePosi(T) & root()
    {
        return _root;
    }
    // 判空
    bool empty() const 
    {
        return !_root;
    }
    // 查找
    BTNodePosi(T) search(const T &e)
    {
        return NULL;
    }
    // 插入
    bool insert(const T &e)
    {
        return true;
    }
    // 删除
    bool remove(const T &e)
    {
        return true;
    }

}; //BTree

}; // MYSTL

#endif