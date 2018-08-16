#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

#include "binSearchTree.h"
namespace MYSTL 
{

// 理想平衡
#define Balanced(x) (stature(x->lChild) == stature(x->rChild))
// 平衡因子
#define BalFac(x) (stature(x->lChild) - stature(x->rChild))
// AVL平衡条件
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))

template <typename T>
class AVL : public BST<T>
{
public:
    // 插入重写基类
    BinNodePosi(T) insert(const T& e)
    {
        // 引用类型
        BinNodePosi(T) & x = search(e);
        // 已存在e值
        if (x) return x;
        BinNodePosi(T) xx = x = new BinNode<T>(e, this->_hot);
        ++this->_size;
        // 重平衡 从父节点向上回溯调整
        for (BinNodePosi(T) g = this->_hot; g; g = g->parent)
        {
            // g失衡 采用"3+4"算法重平衡
            if (!AvlBalanced(g))
            {
                // v p g节点
                FromParentTo(g) = rotateAt(tallerChild(tallerChild(g)));
                // 插入节点调整一次必然平衡
                break;
            }
            // 未失衡更新树高度
            else 
            {
                updateHeight(g);
            }
        }
        return xx;
    }
    // 删除重写基类
    bool remove(const T& e)
    {
        // 引用类型
        BinNodePosi(T)& x = search(e);
        if (!x) return false;
        removeAt(x, this->_hot);
        --this->_size;
        // 重平衡 从父节点向上回溯调整
        for (BinNodePosi(T) g = this->_hot; g; g = g->parent)
        {
            if (!AvlBalanced(g))
            {
                // v p g节点
                g = FromParentTo(g) = rotateAt(tallerChild(tallerChild(g)));
            }
            updateHeight(g);
            // 需要一直向上调整 最坏情况下 调整log(n)次
        }
    }
};

};

#endif