#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <list>

typedef int ElemType;

class TreeNode
{
  public:
    // 树节点保存的值类型
    ElemType obj_;
    // 当前节点的高度
    // 平衡因子需要根据当前节点的两个子树高度差实时计算,不保存在节点结构中
    // 树状态维护
    int height_;
    // 树结构维护
    TreeNode *parent_;
    TreeNode *left_;
    TreeNode *right_;

  public:
    // NULL节点高度为0 叶子节点高度为1
    TreeNode()
        : obj_(ElemType()), height_(1), parent_(NULL), left_(NULL), right_(NULL)
    {

    }
    TreeNode(const ElemType &value)
        : obj_(value), height_(1), parent_(NULL), left_(NULL), right_(NULL)
    {

    }
};

// 中序遍历
void MidTraver(TreeNode *pNode)
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
void MidTraverSave(TreeNode *pNode, std::list<TreeNode *> &l)
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
// 计算树的平衡因子
int GetTreeBalance(TreeNode *pNode)
{
    int lefth = 0;
    int righth = 0;
    if (pNode->left_)
    {
        lefth = pNode->left_->height_;
    }
    if (pNode->right_)
    {
        righth = pNode->right_->height_;
    }
    int diff = lefth - righth;
    if (diff < 0)
    {
        diff *= -1;
    }
    return diff;
}

// 判断指定的子树是否平衡
// 平衡因子
bool IsBalance(TreeNode *pNode)
{
    return (GetTreeBalance(pNode) < 2);
}
int GetBalance(TreeNode *pNode)
{
    return (NULL == pNode) ? 0 : pNode->height_;
}
// 旋转时机
// 旋转的4种模式
// LL
//                  A
//          B
//      NEW
//                  A
//          B               X
//      X       C
// NEW
TreeNode *RotateLL(TreeNode *pNode)
{
    assert(pNode->left_);
    // 维护B
    TreeNode *pRecord = pNode->left_;
    // 维护B父指针
    pRecord->parent_ = pNode->parent_;
    // 维护A父指针
    pNode->parent_ = pNode->left_;
    // 维护A左孩子指针
    pNode->left_ = pNode->left_->right_;
    if (pNode->left_)
    {
        // 维护C父指针
        pNode->left_->parent_ = pNode;
    }
    // 维护B右孩子指针
    pRecord->right_ = pNode;
    // 重设调整后的节点高度
    pNode->height_ = std::max<int>(GetBalance(pNode->left_), GetBalance(pNode->right_)) + 1;
    pRecord->height_ = std::max<int>(GetBalance(pRecord->left_), GetBalance(pRecord->right_)) + 1;
    return pRecord;
}

// RR
//      A
//              B
//                  NEW
//
//      A
// D            B
//          C       X
//                      NEW
TreeNode *RotateRR(TreeNode *pNode)
{
    assert(pNode->right_);
    TreeNode *pRecord = pNode->right_;
    // B的父指针
    pNode->right_->parent_ = pNode->parent_;
    // A的父指针
    pNode->parent_ = pRecord;
    // A的右孩子指针
    pNode->right_ = pNode->right_->left_;
    if (pNode->right_)
    {
        // C的父指针
        pNode->right_->parent_ = pRecord;
    }
    // B的左孩子指针
    pRecord->left_ = pNode;
    // 重设调整后的节点高度
    pNode->height_ = std::max<int>(GetBalance(pNode->left_), GetBalance(pNode->right_)) + 1;
    pRecord->height_ = std::max<int>(GetBalance(pRecord->left_), GetBalance(pRecord->right_)) + 1;
    return pRecord;
}
// LR
TreeNode *RotateLR(TreeNode *pNode)
{
    // 向左边旋转并更新左边指向
    pNode->left_ = RotateRR(pNode->left_);
    // 向右边旋转
    return RotateLL(pNode);
}
// RL
TreeNode *RotateRL(TreeNode *pNode)
{
    // 向右边旋转并跟新右边指向
    pNode->right_ = RotateLL(pNode->right_);
    // 向左边旋转
    return RotateRR(pNode);
}

//
TreeNode *Insert(TreeNode *pNode, const ElemType &value)
{
    TreeNode *pNewNode = new TreeNode(value);
    assert(pNewNode);
    // 出口条件
    if (pNode == NULL)
    {
        pNode = pNewNode;
        return pNode;
    }

    // go + solve
    if (value < pNode->obj_)
    {
        pNode->left_ = Insert(pNode->left_, value);
        // 维护新节点的父指针
        pNode->left_->parent_ = pNode;

        // L
        if (!IsBalance(pNode))
        {
            // LL
            if (pNode->left_ && value < pNode->left_->obj_)
            {
                pNode = RotateLL(pNode);
            }
            // LR
            else if (pNode->left_ && value > pNode->left_->obj_)
            {
                pNode = RotateLR(pNode);
            }
            else
            {
                // nothing
            }
        }
    }
    else if (value > pNode->obj_)
    {
        pNode->right_ = Insert(pNode->right_, value);
        // 维护新节点的父指针
        pNode->right_->parent_ = pNode;

        // R
        if (!IsBalance(pNode))
        {
            // RL
            if (pNode->right_ && value < pNode->right_->obj_)
            {
                pNode = RotateRL(pNode);
            }
            // RR
            else if (pNode->right_ && value > pNode->right_->obj_)
            {
                pNode = RotateRR(pNode);
            }
            else
            {
                // nothing
            }
        }
    }
    else
    {
        // nothing
    }
    // back
    // 重设插入节点后本节点的高度
    pNode->height_ = std::max<int>(GetBalance(pNode->left_), GetBalance(pNode->right_)) + 1;
    return pNode;
}
