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

bool IsBalance(TreeNode *pNode);
// 中序遍历检查树平衡
void MidTraverCheckBalance(TreeNode *pNode)
{
    if (!pNode)
    {
        return;
    }
    // go
    MidTraverCheckBalance(pNode->left_);
    // solve
    assert(IsBalance(pNode));
    // go
    MidTraverCheckBalance(pNode->right_);
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
int GetHeight(TreeNode *pNode)
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
    pNode->height_ = std::max<int>(GetHeight(pNode->left_), GetHeight(pNode->right_)) + 1;
    pRecord->height_ = std::max<int>(GetHeight(pRecord->left_), GetHeight(pRecord->right_)) + 1;
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
    pNode->height_ = std::max<int>(GetHeight(pNode->left_), GetHeight(pNode->right_)) + 1;
    pRecord->height_ = std::max<int>(GetHeight(pRecord->left_), GetHeight(pRecord->right_)) + 1;
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
// 左边高于右边
void ReBalanceLeft(TreeNode* pNode)
{
    TreeNode* l = pNode->left_;
    // 右边高于左边
    if (GetHeight(l->left_) - GetHeight(l->right_) == -1)
    {   
        RotateLR(pNode);
    }
    else 
    {
        RotateLL(pNode);
    }
}
// 右边高于左边
void ReBalanceRight(TreeNode* pNode)
{
    TreeNode* r = pNode->right_;
    // 左边高于右边
    if (GetHeight(r->right_) - GetHeight(r->left_) == -1)
    {
        RotateRL(pNode);
    }
    else 
    {
        RotateRR(pNode);
    }
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
        // if (GetHeight(pNode->left_) - GetHeight(pNode->right_) >= 2)
        // {
        //     ReBalanceLeft(pNode->left_);
        // }
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
        // if (GetHeight(pNode->right_->right_) - GetHeight(pNode->right_->left_) >= 2)
        // {
        //     ReBalanceRight(pNode->right_);
        // }
    }
    else
    {
        // nothing
    }
    // back
    // 重设插入节点后本节点的高度
    pNode->height_ = std::max<int>(GetHeight(pNode->left_), GetHeight(pNode->right_)) + 1;
    return pNode;
}

TreeNode* Search(TreeNode* pNode, const ElemType& value)
{
    // 出口条件
    if (!pNode || pNode->obj_ == value)
    {
        return pNode;
    }
    TreeNode* pCur = NULL;
    if (value <= pNode->obj_)
    {
        pCur = Search(pNode->left_, value);
    }
    else
    {
        pCur = Search(pNode->right_, value);
    }
    return pCur;
}

// // 迭代删除如何调整平衡?
// bool Delete(TreeNode* pNode, const ElemType& value)
// {
//     bool flag = false;
//     TreeNode* pCur = Search(pNode, value);  
//     if (pCur)
//     {  
//         flag = true;
//         // 调整平衡 ???

//         TreeNode* parent = pCur->parent_;
//         // 叶子节点或者根节点
//         if (!pCur->left_ && !pCur->right_)
//         {
//             // 根节点
//             if (!parent)
//             {
//                 delete pCur;
//                 pCur = NULL;
//             }
//             else if (parent->left_ == pCur)
//             {
//                 delete pCur;
//                 parent->left_ = NULL;
//             }
//             else if (parent->right_ == pCur)
//             {
//                 delete pCur;
//                 parent->right_ = NULL;
//             }
//         }
//         // 非叶子节点
//         else 
//         {
//             // 只存在左子树
//             if (pCur->left_ && !pCur->right_)
//             {
//                 TreeNode* pRelease = pCur->left_;
//                 pCur->obj_ = pRelease->obj_;
//                 pCur->left_ = pRelease->left_;
//                 pCur->right_ = pRelease->right_;
//                 delete pRelease;
//                 pRelease = NULL;
//             }
//             // 只存在右子树
//             else if (pCur->right_ && !pCur->left_)
//             {
//                 TreeNode* pRelease = pCur->right_;
//                 pCur->obj_ = pRelease->obj_;
//                 pCur->left_ = pRelease->left_;
//                 pCur->right_ = pRelease->right_;
//                 delete pRelease;
//                 pRelease = NULL;
//             }
//             // 左右子树都存在
//             else 
//             {
//                 // 寻找左子树最大或者右子树最小的值来替换当前需要删除的值
//                 // rMin表示右边最小的值
//                 TreeNode* rMin = pCur->right_;
//                 parent = pCur;
//                 while (rMin->left_)
//                 {
//                     parent = rMin;
//                     rMin = rMin->left_;
//                 }
//                 TreeNode* pRelease = rMin;
//                 pCur->obj_ = pRelease->obj_;
//                 // pCur的直接右孩子存在左孩子
//                 if (parent->left_ == rMin)
//                 {
//                     parent->left_ = rMin->right_; 
//                 }
//                 // pCur的直接右孩子不存在左孩子,则最小的值就是pCur的直接右孩子
//                 else 
//                 {
//                     parent->right_ = rMin->right_;
//                 }
//             }
//         }
//     }
//     return flag;
// }
TreeNode* FindLeftMax(TreeNode* pNode)
{
    TreeNode* lMax = pNode->left_;
    while(lMax->right_)
    {
        lMax = lMax->right_;
    }
    return lMax;
}
TreeNode* FindRightMin(TreeNode* pNode)
{
    TreeNode* rMin = pNode->right_;
    while (rMin->left_)
    {
        rMin = rMin->left_;
    }
    return rMin;
}

// 删除后树的平衡调节
bool Delete(TreeNode* pNode, const ElemType& value)
{
    bool flag = false;
    if (pNode == NULL)
    {
        return false;
    }
    if (value == pNode->obj_)
    {
        // 叶子节点或者根节点
        if (!pNode->left_ && !pNode->right_)
        {
            if (pNode->parent_ && pNode == pNode->parent_->left_)
            {
                pNode->parent_->left_ = NULL;
            }
            else if (pNode->parent_ && pNode == pNode->parent_->right_)
            {
                pNode->parent_->right_ = NULL;
            }
            delete pNode;
            pNode = NULL;
            flag = true;
        }
        // 只存在左子树,转移待删除节点
        else if (pNode->left_ && !pNode->right_)
        {
            TreeNode* lMax = FindLeftMax(pNode);
            assert(lMax);
            pNode->obj_ = lMax->obj_;
            // Delete(pNode->left_, lMax->obj_);
            flag = Delete(lMax, lMax->obj_);
        }
        // 只存在右子树,转移待删除节点
        else if (pNode->right_ && !pNode->left_)
        {
            TreeNode* rMin = FindRightMin(pNode);
            assert(rMin);
            pNode->obj_ = rMin->obj_;
            // Delete(pNode->right_, rMin->obj_);
            flag = Delete(rMin, rMin->obj_);
        }
        // 左右子树都存在,转移待删除节点
        else 
        {
            TreeNode* rMin = FindRightMin(pNode);
            assert(rMin);
            pNode->obj_ = rMin->obj_;
            // Delete(pNode->right_, rMin->obj_);
            flag = Delete(rMin, rMin->obj_);            
        }
    }
    else if (value < pNode->obj_)
    {
        flag = Delete(pNode->left_, value);
        // 删除后右边比左边高
        // todo
    }
    else 
    {
        flag = Delete(pNode->right_, value);
        // 删除后左边比右边高
        // todo
    }
    // 更新节点高度
    if (pNode)
    {
        pNode->height_ = std::max<int>(GetHeight(pNode->left_), GetHeight(pNode->right_)) + 1;
    }
    return flag;
}
