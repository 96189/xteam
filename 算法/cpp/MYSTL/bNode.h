#ifndef _B_NODE_H_
#define _B_NODE_H_

#include "vector.h"

namespace MYSTL
{

#define BTNodePosi(T) BTNode<T>*

// B-树模板类
template <typename T>
class BTNode 
{
public:
// 成员
    BTNodePosi(T) parent;           // 父节点  
    Vector<T> key;                  // 关键码向量
    Vector<BTNodePosi(T)> child;    // 孩子向量(长度总比key多1)
// 构造函数
    BTNode() { parent = NULL; child.insert(0, NULL); }
    BTNode(T e, BTNodePosi(T) lc = NULL, BTNodePosi(T) rc = NULL)
    {
        parent = NULL;
        // 一个关键码
        key.insert(0, e);
        // 两个孩子
        child.insert(0, lc); child.inert(1, rc);
        if (lc) lc->parent = this;
        if (rc) rc->parent = this;
    }
};

};

#endif