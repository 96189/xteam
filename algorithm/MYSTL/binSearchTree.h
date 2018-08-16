#ifndef _BIN_SEARCH_TREE_H_
#define _BIN_SEARCH_TREE_H_

#include "bintree.h"

namespace MYSTL 
{

template <typename T>
class BST : public BinTree<T>
{   
protected:
    // search()
    // hot指向最终匹配节点的父节点 若树中无匹配节点则指向搜素中最后访问的节点
    BinNodePosi(T) _hot;
    // 按照"3+4"结构 连接3个节点及4棵子树
    BinNodePosi(T) connect34(
        BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
        BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
    )
    {
        a->lChild = T0; if (T0) T0->parent = a;
        a->rChild = T1; if (T1) T1->parent = a; updateHeight(a);

        c->lChild = T2; if (T2) T2->parent = c;
        c->rChild = T3; if (T3) T3->parent = c; updateHeight(c);
        
        b->lChild = a; a->parent = b;
        b->rChild = c; c->parent = b; updateHeight(b);
        // 新子树的根节点
        return b;
    }
    // 对x及其父节点 祖父节点统一做旋转调整
    BinNodePosi(T) rotateAt(BinNodePosi(T) v)
    {
        // 从参数种取出 p g节点
        BinNodePosi(T) p = v->parent;
        BinNodePosi(T) g = p->parent;
        // 视v p g的4种情况映射v p g到 a < b < c
        if (IsLChild(p))
        {
            // 左-左直线
            if (IsLChild(v))
            {
                p->parent = g->parent;
                return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
            }
            // 左-右折线
            else 
            {
                v->parent = g->parent;
                return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
            }
        }
        else 
        {
            // 右-右直线
            if (IsRChild(v))
            {
                p->parent = g->parent;
                return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
            }
            // 右-左折线
            else 
            {
                v->parent = g->parent;
                return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
            }
        }
    }
public:
    // 基本接口:以virtua修饰 强制要求所有派生类根据各自的规则对其重写
    // 查找
    virtual BinNodePosi(T) &search(const T& e)
    {
        // 返回目标节点的引用 以便后序插入删除
        return searchIn(this->_root, e, _hot = NULL);
    }
    // 插入
    // 子类重写本函数 接收search()的变量必须是引用变量
    virtual BinNodePosi(T) insert(const T& e)
    {
        // 变量为引用
        BinNodePosi(T)& p = search(e);
        // 已存在直接返回
        if (p) return p;
        p = new BinNode<T>(e, _hot);
        ++this->_size;
        updateHeightAbove(_hot);
        return p;
    }
    // 删除
    // 子类重写本函数 接收search()的变量必须是引用变量
    virtual bool remove(const T& e)
    {
        // 变量为引用
        BinNodePosi(T)& p = search(e);
        // 值不存在
        if (!p) return false;
        // 
        removeAt(p, _hot);
        --this->_size;
        updateHeightAbove(_hot);
        return true;
    }
};

};

#endif