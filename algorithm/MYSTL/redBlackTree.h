#ifndef _RED_BLACK_TREE_H_
#define _RED_BLACK_TREE_H_

#include <algorithm>
#include "binSearchTree.h"

namespace MYSTL 
{

template <typename T>
class RBTree : public BST<T>
{
protected:
    // 双红修正
    // RedBlack双红调整算法：解决节点x与其父均为红色的问题。分为两大类情况：
    //    RR-1：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
    //    RR-2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
    void solveDoubleRed(BinNodePosi(T) x)
    {
        // 已递归到树根 将其转黑 整棵树黑高度也 随之递增
        if (IsRoot(x))
        {
            this->_root->color = RB_BLACK;
            ++this->_root->height;
            return;
        }
        // 父节点为黑 可立即返回 结束调整
        BinNodePosi(T) p = x->parent;
        if (IsBlack(p)) return;

        BinNodePosi(T) g = p->parent;   // g比存在且必黑
        BinNodePosi(T) u = uncle(x);
        // 叔父节点为黑色 或NULL
        // 类似于 提升变换后的(2,4)B树 
        // 当前超级节点中存在红红黑三个节点 至需要将颜色修改为红黑红
        // 旋转一次 调整结构 将黑节点旋转至树根(现在的树根节点为g为红色)
        if (IsBlack(u))
        {
            // 直线型
            if (IsLChild(x) == IsLChild(p))
                p->color = RB_BLACK;
            // 折线型
            else 
                x->color = RB_BLACK;
            // g由黑 转红
            g->color = RB_RED;
            BinNodePosi(T) gg = g->parent;
            // 树结构调整一次 3+4重构
            BinNodePosi(T) r = FromParentTo(g) = rotateAt(x);
            // 调整之后的新子树 与原曾祖父连接
            r->parent = gg;
        }
        // 叔父节点为红色
        // 类似于 提升变换后的(2,4)B树发生上溢
        // 需要完成一次上溢修正 将超级节点中位数位置的值 向上插入
        else 
        {
            // p由红转黑 增高
            p->color = RB_BLACK;
            ++p->height;
            // u由红转黑 增高
            u->color = RB_BLACK;
            ++u->height;
            // g若非根 则转红
            if (!IsRoot(g)) g->color = RB_RED;
            // 继续调整g
            solveDoubleRed(g);
        }
    }
    // 双黑修正
    // RedBlack双黑调整算法：解决节点x与被其替代的节点均为黑色的问题
    // 分为三大类共四种情况：
    //    BB-1 ：2次颜色翻转，2次黑高度更新，1~2次旋转，不再递归
    //    BB-2R：2次颜色翻转，2次黑高度更新，0次旋转，不再递归
    //    BB-2B：1次颜色翻转，1次黑高度更新，0次旋转，需要递归
    //    BB-3 ：2次颜色翻转，2次黑高度更新，1次旋转，转为BB-1或BB2R
    void solveDoubleBlack(BinNodePosi(T) r)
    {
        BinNodePosi(T) p = r ? r->parent : this->_hot;
        if (!p)
            return;                                      //r的父亲
        BinNodePosi(T) s = (r == p->lChild) ? p->rChild : p->lChild; //r的兄弟
        if (IsBlack(s))
        {                            //兄弟s为黑
            BinNodePosi(T) t = NULL; //s的红孩子（若左、右孩子皆红，左者优先；皆黑时为NULL）
            if (IsRed(s->rChild))
                t = s->rChild; //右子
            if (IsRed(s->lChild))
                t = s->lChild; //左子
            if (t)
            {                                                      //黑s有红孩子：BB-1
                RBColor oldColor = p->color;                       //备份原子树根节点p颜色，并对t及其父亲、祖父
                                                                   // 以下，通过旋转重平衡，并将新子树的左、右孩子染黑
                BinNodePosi(T) b = FromParentTo(p) = rotateAt(t); //旋转
                if (HasLChild(b))
                {
                    b->lChild->color = RB_BLACK;
                    updateHeight(b->lChild);
                } //左子
                if (HasRChild(b))
                {
                    b->rChild->color = RB_BLACK;
                    updateHeight(b->rChild);
                } //右子
                b->color = oldColor;
                updateHeight(b); //新子树根节点继承原根节点的颜色
            }
            else
            { //黑s无红孩子
                s->color = RB_RED;
                s->height--; //s转红
                if (IsRed(p))
                {                        //BB-2R
                    p->color = RB_BLACK; //p转黑，但黑高度不变
                }
                else
                {                        //BB-2B
                    p->height--;         //p保持黑，但黑高度下降
                    solveDoubleBlack(p); //递归上溯
                }
            }
        }
        else
        { //兄弟s为红：BB-3
            s->color = RB_BLACK;
            p->color = RB_RED;                               //s转黑，p转红
            BinNodePosi(T) t = IsLChild(s) ? s->lChild : s->rChild; //取t与其父s同侧
            this->_hot = p;
            FromParentTo(p) = rotateAt(t); //对t及其父亲、祖父做平衡调整
            solveDoubleBlack(r);            //继续修正r处双黑——此时的p已转红，故后续只能是BB-1或BB-2R
        }
    }
    // 更新节点x的高度(黑高度)
    int updateHeight(BinNodePosi(T) x)
    {
        x->height = std::max<T>(stature(x->lChild), stature(x->rChild));
        if (IsBlack(x))
            ++x->height;
        return x->height;
    }
public:
    BinNodePosi(T) insert(const T &e)
    {
        BinNodePosi(T) & x = search(e);
        if (x) return x;
        // 创建红节点x 以this->_hot为父 黑高度-1
        x = new BinNode<T>(e, this->_hot, NULL, NULL, -1);
        ++this->_size;
        // 如果有必要 做双红修正
        solveDoubleRed(x);
        return x ? x : this->_hot->parent;
    }
    bool remove(const T &e)
    {
        BinNodePosi(T) & x = search(e);
        if (!x) return false;
        // r为x在树中(中序遍历意义下的)直接后继 x存储r的值后
        // 实际删除的节点为r
        BinNodePosi(T) r = removeAt(x, this->_hot);
        // 删除后树空
        if (!(--this->_size)) return true;
        // 被删除的为树根
        if (!this->_hot)
        {
            this->_root->color = RB_BLACK;
            updateHeight(this->_root);
            return true;
        }
        // 父亲及祖先依然平衡
        if (BlackHeightUpdated(this->_hot)) return true;
        // 替代节点r为红 则只需要简单的翻转其颜色
        if (IsRed(r))
        {
            r->color = RB_BLACK;
            ++r->height;
            return true;
        }
        // 双黑调整
        solveDoubleBlack(r);
        return true;
    }
};

};

#endif