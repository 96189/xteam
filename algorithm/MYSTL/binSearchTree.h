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
    // 按照"3+4"结构 连接3各节点及4棵子树
    BinNodePosi(T) connect34(
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
        BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T)
    );
    // 对x及其父节点 祖父节点统一做旋转调整
    BinNodePosi(T) rotateAt(BinNodePosi(T) x);
public:
    // 基本接口:以virtua修饰 强制要求所有派生类根据各自的规则对其重写
    // 查找
    virtual BinNodePosi(T) &search(const T& e)
    {
        // 返回目标节点的引用 以便后序插入删除
        return searchIn(this->_root, e, _hot = NULL);
    }
    // 插入
    virtual BinNodePosi(T) insert(const T& e)
    {
        BinNodePosi(T) p = search(e);
        // 已存在直接返回
        if (p) return p;
        if (e < _hot->data)
        {
            return insertAsLC(_hot, e);
        }
        else if (_hot->data < e)
        {
            return insertAsRC(_hot, e);
        }
        else 
        {
            assert(false);
            return NULL;
        }
    }
    // 删除
    virtual bool remove(const T& e)
    {
        BinNodePosi(T) p = search(e);
        // 值不存在
        if (!p) return false;
        // 
        removeAt(p, _hot);
        return true;
    }
};

};

#endif