#ifndef _SPLAY_TREE_H_
#define _SPLAY_TREE_H_

#include "binSearchTree.h"
namespace MYSTL 
{

template <typename T>
class Splay : public BST<T>
{
protected:
    // 从节点v出发伸展
    BinNodePosi(T) splay(BinNodePosi(T) v)
    {
        if (!v) return NULL;
        // 祖父节点与父节点
        BinNodePosi(T) g = NULL;
        BinNodePosi(T) p = NULL;
        // 自下而上 对v做双层伸展
        while ((p = v->parent) && (g = p->parent))
        {
            BinNodePosi(T) gg = g->parent; // 每轮以后v都以原曾祖父为父 这里先记录下来
            if (IsLChild(v))
            {
                // 左侧直线
                if (IsLChild(p))
                {
                    // 伸展树双层旋转的特点 从祖父节点开始旋转
                    // 子树重接
                    BinTree<T>::attachAsLC(g, p->rChild);
                    BinTree<T>::attachAsLC(p, v->rChild);
                    // g做p的右孩子
                    BinTree<T>::attachAsRC(p, g);
                    // p做v的左孩子
                    BinTree<T>::attachAsRC(v, p);
                }
                // 左侧折线
                else 
                {
                    // 与AVL双旋转一样
                    BinTree<T>::attachAsLC(p, v->rChild);
                    BinTree<T>::attachAsRC(g, v->lChild);
                    BinTree<T>::attachAsLC(v, g);
                    BinTree<T>::attachAsRC(v, p);
                }
            }
            // 右侧直线
            else if (IsRChild(p))
            {
                // 伸展树双层旋转的特点 从祖父节点开始旋转
                // 子树重接
                BinTree<T>::attachAsRC(g, p->lChild);
                BinTree<T>::attachAsRC(p, v->lChild);
                // g做p的左孩子
                BinTree<T>::attachAsLC(p, g);
                // p做v的左孩子
                BinTree<T>::attachAsLC(v, p);
            }
            // 右侧折线
            else 
            {
                // 与AVL双旋转一样
                BinTree<T>::attachAsRC(p, v->lChild);
                BinTree<T>::attachAsLC(g, v->rChild);
                BinTree<T>::attachAsRC(v, g);
                BinTree<T>::attachAsLC(v, p);
            }
            // 曾祖父为空
            if (!gg) v->parent = NULL;
            // 曾祖父不为空
            else 
                (g == gg->lChild) ? BinTree<T>::attachAsLC(gg, v) : BinTree<T>::attachAsRC(gg, v);
            // 调整完更新3个节点的高度
            BinTree<T>::updateHeight(g);
            BinTree<T>::updateHeight(p);
            BinTree<T>::updateHeight(v);
        } // 双层伸展结束 必有g == NULL 但p可能非空
        // 若p非空 额外再做一次单旋转
        p = v->parent;
        if (p)
        {
            if (IsLChild(v)) { BinTree<T>::attachAsLC(p, v->rChild); BinTree<T>::attachAsRC(v, p); }
            else { BinTree<T>::attachAsRC(p, v->lChild); BinTree<T>::attachAsLC(v, p); }
            BinTree<T>::updateHeight(p);
            BinTree<T>::updateHeight(v);
        }
        // v到达根节点 父指针置空
        v->parent = NULL;
        return v;
    }
public:
    // 在伸展树中查找关键码e
    BinNodePosi(T) &search(const T& e)
    {
        BinNodePosi(T) &p = BinTree<T>::searchIn(this->_root, e, this->_hot=NULL);
        // 将最后一个被访问的节点伸展至树根
        this->_root = splay(p ? p : this->_hot);
        return this->_root;
    }
    // 将关键码e插入伸展树中 最终位置在树根
    BinNodePosi(T) insert(const T& e)
    {
        // 树为空
        if (!this->_root)
        {
            ++this->_size;
            return this->_root = new BinNode<T>(e);
        }
        // 关键码e已存在
        if (e == search(e)->data) return this->_root;
        // 关键码e不存在 
        // 上一步search已经对树做了伸展 不存在会将最后 一个 访问的节点伸展的树根
        this->_size++;
        // 创建新节点 以下调整<=7个节点完成局部重构
        BinNodePosi(T) t = this->_root;
        // 插入新根 以t和t->rChild为 左右孩子
        if (this->_root->data < e)
        {
            t->parent = this->_root = new BinNode<T>(e, NULL, t, t->rChild); // 2+3个
            if (HasRChild(t))
            {
                // <= 2个
                t->rChild->parent = this->_root;
                t->rChild = NULL;
            }
        }
        // 插入新根 以t和t->lChild为左右孩子
        else 
        {
            t->parent = this->_root = new BinNode<T>(e, NULL, t->lChild, t); // 2+3个
            if (HasLChild(t))
            {
                // <= 2个
                t->lChild->parent = this->_root;
                t->lChild = NULL;
            }
        }
        BinTree<T>::updateHeightAbove(t);
        return this->_root;
    }
    // 从伸展树中删除关键码e
    bool remove(const T& e)
    {
        // 空树或关键码不存在
        if (!this->_root || (e != search(e)->data)) return false;
        // 上一步搜索后关键码e已被伸展至根节点
        BinNodePosi(T) w = this->_root;
        // 无左子树直接删除
        if (!HasLChild(this->_root))
        {
            this->_root = this->_root->rChild;
            if (this->_root)
                this->_root->parent = NULL;
        }
        // 无右子树直接删除
        else if (!HasRChild(this->_root))
        {
            this->_root = this->_root->lChild;
            if (this->_root)
                this->_root->parent = NULL;
        }
        // 左右子树同时存在
        else 
        {
            BinNodePosi(T) lTree = this->_root->lChild;
            lTree->parent = NULL; this->_root->lChild = NULL; // 暂时分离左子树
            this->_root = this->_root->rChild; this->_root->parent = NULL; // 丢掉根节点 只保留右子树
            search(w->data); // 以根节点为目标 做一次必定失败的查找
            // 至此 右子树中最小的节点必定伸展至树根 且 其左子树必空
            // 重新接入左子树
            this->_root->lChild = lTree; lTree->parent = this->_root;
        }
        delete w;
        --this->_size;
        if (this->_root)
            BinTree<T>::updateHeight(this->_root);
        return true;
    }
};

};

#endif