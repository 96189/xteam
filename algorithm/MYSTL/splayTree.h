#ifndef _SPLAY_TREE_H_
#define _SPLAY_TREE_H_

#include "binSearchTree.h"
namespace MYSTL 
{

template <typename T>
class Splay : public BST<T>
{
protected:
    // 将节点v伸展至树根
    BinNodePosi(T) splay(BinNodePosi(T) v);
public:
    BinNodePosi(T) &search(const T& e)
    {
        return NULL;
    }
    BinNodePosi(T) insert(const T& e)
    {
        return NULL;
    }
    bool remove(const T& e)
    {
        return true;
    }
};

};

#endif