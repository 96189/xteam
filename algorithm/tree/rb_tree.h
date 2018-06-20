#include <unistd.h>
#include <stdio.h>
#include <assert.h>
// #include <algorithm>
#include <list>

typedef int ElemType;

class RBTNode
{
  public:
    enum COLOR
    {
        RED,
        BLACK,
    };

  public:
    //
    ElemType obj_;
    // 树状态维护
    COLOR color_;
    // 树结构维护
    RBTNode *parent_;
    RBTNode *left_;
    RBTNode *right_;

  public:
    RBTNode()
        : obj_(ElemType()), color_(RED), parent_(NULL), left_(NULL), right_(NULL)
    {
    }
    RBTNode(const ElemType &value)
        : obj_(value), color_(RED), parent_(NULL), left_(NULL), right_(NULL)
    {
    }
};

// https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91
// 红黑树是每个节点都带有颜色属性的二叉查找树，颜色为红色或黑色。在二叉查找树强制一般要求以外，对于任何有效的红黑树我们增加了如下的额外要求：
// 1、节点是红色或黑色。
// 2、根是黑色。
// 3、所有叶子都是黑色（叶子是NIL节点）。
// 4、每个红色节点必须有两个黑色的子节点。（从每个叶子到根的所有路径上不能有两个连续的红色节点。）
// 5、从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。

// 祖父节点 G
RBTNode *GrandParent(RBTNode *pNode)
{
    assert(pNode->parent_);
    return pNode->parent_->parent_;
}
// 叔叔节点 U
RBTNode *Uncle(RBTNode *pNode)
{
    if (pNode->parent_ == GrandParent(pNode)->left_)
    {
        return GrandParent(pNode)->right_;
    }
    else
    {
        return GrandParent(pNode)->left_;
    }
}
// 父节点 P

// 兄弟节点 S
RBTNode *Sibling(RBTNode *pNode)
{
    assert(pNode->parent_);
    if (pNode->parent_->left_ == pNode)
    {
        return pNode->parent_->right_;
    }
    else
    {
        return pNode->parent_->left_;
    }
}

// // gp fa pNode直线
// //                  gp
// //              fa
// //      pNode
// //              r
// //
// // gp fa pNode折线
// //                  gp
// //                      fa
// //                   pNode
// //                        r
// void RotateR(RBTNode* pNode, RBTNode** root)
// {
//     if (!pNode->parent_)
//     {
//         *root = pNode;
//         return;
//     }

//     RBTNode* gp = GrandParent(pNode);
//     RBTNode* fa = pNode->parent_;
//     RBTNode* r = pNode->right_;
//     // 调整fa左孩子
//     fa->left_ = r;
//     if (NULL != r)
//     {
//         r->parent_ = fa;
//     }
//     // 调整pNode右孩子
//     pNode->right_ = fa;
//     fa->parent_ = pNode;

//     if (*root == fa)
//     {
//         *root = pNode;
//     }
//     pNode->parent_ = gp;
//     if (NULL != gp)
//     {
//         // 调整gp的孩子
//         if (gp->left_ == fa)
//         {
//             gp->left_ = pNode;
//         }
//         else
//         {
//             gp->right_ = pNode;
//         }
//     }
// }
// // gp fa pNode直线
// // gp
// //      fa
// //          pNode
// //      l
// //
// // gp fa pNode折线
// //      gp
// // fa
// //      pNode
// //  l
// void RotateL(RBTNode* pNode, RBTNode** root)
// {
//     if (!pNode->parent_)
//     {
//         *root = pNode;
//         return;
//     }
//     RBTNode* gp = GrandParent(pNode);
//     RBTNode* fa = pNode->parent_;
//     RBTNode* l = pNode->left_;
//     // 调整fa节点
//     fa->right_ = l;
//     if (NULL != l)
//     {
//         l->parent_ = fa;
//     }
//     // 调整pNode的left
//     pNode->left_ = fa;
//     fa->parent_ = pNode;

//     if (*root == fa)
//     {
//         *root = pNode;
//     }
//     pNode->parent_ = gp;
//     if (NULL != gp)
//     {
//         // 调整gp的孩子
//         if (gp->left_ == fa)
//         {
//             gp->left_ = pNode;
//         }
//         else
//         {
//             gp->right_ = pNode;
//         }
//     }
// }
// stl rb_tree 旋转的实现
// parent
//      pNode
//              r
void RotateLeft(RBTNode *pNode, RBTNode **root)
{
    RBTNode *r = pNode->right_;
    pNode->right_ = r->left_;
    if (r->left_)
    {
        r->left_->parent_ = pNode;
    }
    // 维护r的新父节点指针
    r->parent_ = pNode->parent_;
    if (*root == pNode)
    {
        *root = r;
    }
    else if (pNode == pNode->parent_->left_)
    {
        pNode->parent_->left_ = r;
    }
    else
    {
        pNode->parent_->right_ = r;
    }
    r->left_ = pNode;
    pNode->parent_ = r;
}
void RotateRight(RBTNode *pNode, RBTNode **root)
{
    RBTNode *l = pNode->left_;
    pNode->left_ = l->left_;
    if (l->right_)
    {
        l->right_->parent_ = pNode;
    }
    // 维护l的父节点指针
    l->parent_ = pNode->parent_;
    if (*root == pNode)
    {
        *root = l;
    }
    else if (pNode == pNode->parent_->right_)
    {
        pNode->parent_->right_ = l;
    }
    else
    {
        pNode->parent_->left_ = l;
    }
    l->right_ = pNode;
    pNode->parent_ = l;
}

// 新节点颜色为红色
// void InsertCase1(RBTNode* pNode)
// {
//     // 根节点
//     if (NULL == pNode->parent_)
//     {
//         // rule 2
//         pNode->color_ = BLACK;
//     }
//     // 非根节点
//     else
//     {
//         InsertCase2(pNode);
//     }
// }

// //
// void InsertCase2(RBTNode* pNode)
// {
//     // rule 4
//     // 当前路径无连续的红色节点
//     if (pNode->parent_->color_ == BLACK)
//     {
//         // nothing
//     }
//     // 当前路径存在连续的红色节点
//     else
//     {
//         InsertCase3(pNode);
//     }
// }

// void InsertCase3(RBTNode* pNode)
// {
//     // rule 5
//     // 当前节点为红色,父节点为红色,叔叔节点也为红色
//     if (NULL != Uncle(pNode) && Uncle(pNode)->color_ == RED)
//     {
//         pNode->parent_->color_ = BLACK;
//         Uncle(pNode)->color_  = BLACK;
//         GrandParent(pNode)->color_ = RED;

//         // 递归 从祖父节点开始重新调整
//         InsertCase1(GrandParent(pNode));
//     }
//     // 当前节点为红色,父节点为红色,叔叔节点为黑色,或者不存在叔叔节点
//     else
//     {
//         InsertCase4(pNode);
//     }
// }

// // 当前节点为红色,父节点为红色
// void InsertCase4(RBTNode* pNode)
// {
//     // 当前节点是父节点的右孩子 且 父节点是祖父节点的左孩子
//     if (pNode == pNode->parent_->right_ && pNode->parent_ == GrandParent(pNode)->left_)
//     {
//         RotateL(pNode->parent_);
//         pNode = pNode->left_;
//     }
//     // 当前节点是父节点的左孩子 且父节点是祖父节点的右孩子
//     else if (pNode == pNode->parent_->left_ && pNode->parent_ == GrandParent(pNode)->right_)
//     {
//         RotateR(pNode->parent_);
//         pNode = pNode->right_;
//     }
//     //
//     InsertCase5(pNode);
// }

// // 当前节点为红色,父节点为红色
// void InsertCase5(RBTNode* pNode)
// {
//     pNode->parent_->color_ = BLACK;
//     GrandParent(pNode)->color_ = RED;
//     // 当前节点是父节点的左孩子 且 父节点也是祖父节点的左孩子
//     if (pNode == pNode->parent_->left_ && pNode->parent_ == GrandParent(pNode)->left_)
//     {
//         RotateR(GrandParent(pNode));
//     }
//     // 当前节点是父节点的右孩子 且 父节点也是祖父节点的右孩子
//     else
//     {
//         RotateL(GrandParent(pNode));
//     }
// }

void InsertCase(RBTNode *pNode, RBTNode **root)
{
    // 根节点
    if (!pNode->parent_)
    {
        *root = pNode;
        pNode->color_ = RBTNode::BLACK;
        return;
    }

    // 当前节点为红色,父节点为红色
    // rule 4不符合
    if (*root != pNode && pNode->parent_->color_ == RBTNode::RED)
    {
        // 叔叔节点为红色
        if (Uncle(pNode) && Uncle(pNode)->color_ == RBTNode::RED)
        {
            pNode->parent_->color_ = Uncle(pNode)->color_ = RBTNode::BLACK;
            GrandParent(pNode)->color_ = RBTNode::RED;

            InsertCase(GrandParent(pNode), root);
        }
        // 叔叔节点为黑色或者不存在叔叔节点(NIL节点也为黑色)
        // 父节点红色 叔节点黑色
        else
        {
            // 维持树的平衡 旋转
            // 当前节点是父节点的右孩子 父节点是祖父节点的左孩子
            // 折线型
            if (pNode == pNode->parent_->right_ && pNode->parent_ == GrandParent(pNode)->left_)
            {
                // // 左旋转一次后占据pNode的fa位置
                // RotateL(pNode, root);
                // // 右旋转一次后占据pNode的gp位置
                // RotateR(pNode, root);

                // 左旋转完成之后,pNode的新父节点是pNode->right_
                RotateLeft(pNode, root);
                pNode->color_ = RBTNode::BLACK;
                pNode->left_->color_ = pNode->right_->color_ = RBTNode::RED;
                // 此处的参数是原来pNode的parent,由于经过了第一次左旋转,现在是pNode的GrandParent
                // 直线
                RotateRight(pNode->parent_->parent_, root);
            }
            // 当前节点是父节点的左孩子 父节点是祖父节点的右孩子
            // 折线型
            else if (pNode == pNode->parent_->left_ && pNode->parent_ == GrandParent(pNode)->right_)
            {
                // RotateR(pNode, root);
                // RotateL(pNode, root);

                // 右旋完成后,pNode的新父节点是pNode->left_
                RotateRight(pNode, root);
                pNode->color_ = RBTNode::BLACK;
                pNode->left_->color_ = pNode->right_->color_ = RBTNode::RED;
                // 直线
                RotateLeft(pNode->parent_->parent_, root);
            }
            // 当前节点是父节点的左孩子 父节点是祖父节点的左孩子
            // 直线
            else if (pNode == pNode->parent_->left_ && pNode->parent_ == GrandParent(pNode)->left_)
            {
                pNode->parent_->color_ = RBTNode::BLACK;
                GrandParent(pNode)->color_ = RBTNode::RED;
                // RotateR(pNode->parent_, root);
                // 直线
                RotateRight(pNode->parent_->parent_, root);
            }
            // 当前节点是父节点的右孩子 父节点是祖父节点的右孩子
            // 直线
            else if (pNode == pNode->parent_->right_ && pNode->parent_ == GrandParent(pNode)->right_)
            {
                pNode->parent_->color_ = RBTNode::BLACK;
                GrandParent(pNode)->color_ = RBTNode::RED;
                // RotateL(pNode->parent_, root);
                // 直线
                RotateLeft(pNode->parent_->parent_, root);
            }
        }
    }
}

void InsertValue(RBTNode *pNode, const ElemType &value, RBTNode **root)
{
    // L
    if (value < pNode->obj_)
    {
        if (NULL != pNode->left_)
        {
            InsertValue(pNode->left_, value, root);
        }
        else
        {
            RBTNode *pNewNode = new RBTNode(value);
            assert(pNewNode);
            pNewNode->parent_ = pNode;
            pNode->left_ = pNewNode;
            InsertCase(pNewNode, root);
        }
    }
    // R
    else if (value > pNode->obj_)
    {
        if (NULL != pNode->right_)
        {
            InsertValue(pNode->right_, value, root);
        }
        else
        {
            RBTNode *pNewNode = new RBTNode(value);
            assert(pNewNode);
            pNewNode->parent_ = pNode;
            pNode->right_ = pNewNode;
            InsertCase(pNewNode, root);
        }
    }
    else
    {
        // nothing
    }
}

void Insert(RBTNode **root, const ElemType &value)
{
    if ((*root) == NULL)
    {
        *root = new RBTNode(value);
        (*root)->color_ = RBTNode::BLACK;
    }
    else
    {
        InsertValue(*root, value, root);
    }
}
// 中序遍历
void MidTraver(RBTNode *pNode)
{
    if (!pNode)
    {
        return;
    }
    // go
    MidTraver(pNode->left_);
    // solve
    printf("%d -> ", pNode->obj_);
    // go
    MidTraver(pNode->right_);
    // back
}
void MidTraverSave(RBTNode *pNode, std::list<RBTNode *> &l)
{
    if (!pNode)
    {
        return;
    }
    // go
    MidTraverSave(pNode->left_, l);
    // solve
    l.push_back(pNode);
    // go
    MidTraverSave(pNode->right_, l);
    // back
}