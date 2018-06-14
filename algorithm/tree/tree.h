#include <unistd.h>
#include <assert.h>
#include <stdio.h>

typedef int ElemType;

class TreeNode
{
public:
    ElemType obj_;
    TreeNode* left_;
    TreeNode* right_;
public:
    TreeNode()
    {
        obj_ = 0;
        left_ = right_ = NULL;
    }
    TreeNode(const ElemType& value)
    {
        obj_ = value;
        left_ = right_ = NULL;
    }
};

// void BuildTree(TreeNode* root, std::vector<ElemType>& vec)
// {   
//     for (std::vector<ElemType>::iterator it = vec.begin(); it != vec.end(); ++it)
//     {
//         Insert(root, *it);
//     }
// }

// 二叉查找树插入(二叉排序树、二叉搜索树)
// 根节点为空,生成根节点
// 小于等于父节点插左边
// 大于父节点插右边
// 中序遍历得到有序序列
TreeNode* Insert(TreeNode* pNode, const ElemType& value)
{
    TreeNode* pNewNode = new TreeNode(value);
    assert(pNewNode);
    // 出口条件
    if (pNode == NULL)
    {
        pNode = pNewNode;
        return pNode;
    }

    // go + solve
    if (value <= pNode->obj_)
    {
        pNode->left_ = Insert(pNode->left_, value);
    }
    else 
    {
        pNode->right_ = Insert(pNode->right_, value);
    }
    // back
    return pNode;
}

TreeNode* Search(TreeNode* pNode, const ElemType& value, TreeNode** parent)
{
    // 出口条件
    if (!pNode || pNode->obj_ == value)
    {
        return pNode;
    }
    TreeNode* pCur = NULL;
    if (value <= pNode->obj_)
    {
        *parent = pNode;
        pCur = Search(pNode->left_, value, parent);
    }
    else
    {
        *parent = pNode;
        pCur = Search(pNode->right_, value, parent);
    }
    return pCur;
}

// 二叉排序树中最大的节点
// pMax节点一定是右边叶子节点 或者 倒数第二层的只存在左孩子的非叶子节点
TreeNode* BSTMaxTreeNode(TreeNode* pNode, TreeNode** parent)
{
    assert(pNode);
    TreeNode* pMax = pNode;
    while (pNode && pNode->right_)
    {
        *parent = pNode;
        pMax = pNode->right_;
        pNode = pNode->right_;
    }
    return pMax;
}

// 搜索待删除数据在树中的位置
// 删除节点实际是交换待删除节点和被选中节点的内部值 然后删除被选中的节点
// 因此即使删除根节点原来的根节点指针依然有效
bool Delete(TreeNode* pNode, const ElemType& value)
{
    bool flag = false;
    TreeNode* parent = NULL;
    TreeNode* pCur = Search(pNode, value, &parent);
    if (pCur)
    {
        // 叶子节点
        if (!pCur->left_ && !pCur->right_)
        {
            if (parent->left_ == pCur)
            {
                delete pCur;
                parent->left_ = NULL;
            }
            else if (parent->right_ == pCur)
            {
                delete pCur;
                parent->right_ = NULL;
            }
        }
        // 非叶子节点
        else 
        {
            // 二叉搜索的性质 => 优先处理左 然后处理右
            // 存在左子树 
            // 左子树最大的节点与之互换 删除左子树中最大的节点
            if (pCur->left_)
            {
                // 转移待删除节点到pMax节点
                TreeNode* parent = pCur;
                TreeNode* pMax = BSTMaxTreeNode(pCur->left_, &parent);
                assert(pMax);
                pCur->obj_ = pMax->obj_;
                // 删除pMax节点
                // pMax节点为叶子节点,需要维护父节点的右孩子指针
                if (!pMax->right_ && !pMax->left_)
                {
                    delete pMax;
                    pMax = NULL;
                    if (parent != pCur)
                    {
                        parent->right_ = NULL;
                    }
                    // 待删除节点的直接左孩子是被选中的节点
                    else if (parent == pCur)
                    {
                        parent->left_ = NULL;
                    }
                }
                // pMax节点为非叶子节点
                else
                {
                    pMax->obj_ = pMax->left_->obj_;
                    delete pMax->left_;
                    pMax->left_ = NULL;
                }
            }
            // 不存在左子树
            // 存在右子树 右子树最小的节点与之互换 删除右子树中最大的节点
            else if (pCur->right_)
            {
                TreeNode* release = pCur->right_;
                // 转移待删除节点到右子树根节点
                pCur->obj_ = pCur->right_->obj_;
                pCur->right_ = pCur->right_->right_;
                delete release;
                release = NULL;
            }
        }
        flag = true;
    }
    return flag;
}

// 先序遍历
void FirstTraver(TreeNode* pNode)
{
    // 出口条件
    if (!pNode)
    {
        return;
    }
    // solve
    printf("%d -> ", pNode->obj_);
    // go
    FirstTraver(pNode->left_);
    FirstTraver(pNode->right_);
    // back
}

// 中序遍历
void MidTraver(TreeNode* pNode)
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

// 后序遍历
void LastTraver(TreeNode* pNode)
{
    if (!pNode)
    {
        return;
    }
    // go
    LastTraver(pNode->left_);
    LastTraver(pNode->right_);
    // back
    // solve
    printf("%d -> ", pNode->obj_);
}

// 深度：对于任意节点n,n的深度为从根到n的唯一路径长,根的深度为0
int Depth(TreeNode* pNode)
{
    if (!pNode || (!pNode->left_ && !pNode->right_))
    {
        return 0;
    }
    return (Depth(pNode->left_) > Depth(pNode->right_) ? (Depth(pNode->left_) + 1) : (Depth(pNode->right_) + 1));
}