#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <vector>
#include <list>
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
    ~TreeNode()
    {
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
        // 叶子节点或者根节点
        if (!pCur->left_ && !pCur->right_)
        {
            // 根节点
            if (!parent)
            {
                delete pCur;
                pCur = NULL;
            }
            else if (parent->left_ == pCur)
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
            // 只存在左子树
            if (pCur->left_ && !pCur->right_)
            {
                TreeNode* pRelease = pCur->left_;
                pCur->obj_ = pRelease->obj_;
                pCur->left_ = pRelease->left_;
                pCur->right_ = pRelease->right_;
                delete pRelease;
                pRelease = NULL;
            }
            // 只存在右子树
            else if (pCur->right_ && !pCur->left_)
            {
                TreeNode* pRelease = pCur->right_;
                pCur->obj_ = pRelease->obj_;
                pCur->left_ = pRelease->left_;
                pCur->right_ = pRelease->right_;
                delete pRelease;
                pRelease = NULL;
            }
            // 左右子树都存在
            else 
            {
                // 左子树中找最大值 或者 右子树中找最小值
                // 节点转移后删除
                TreeNode* rMin = pCur->right_;
                parent = pCur;
                while (rMin->left_)
                {
                    parent = rMin;
                    rMin = rMin->left_;
                }
                TreeNode* pRelease = rMin;
                pCur->obj_ = rMin->obj_;
                if (parent->left_ == rMin)
                {
                    parent->left_ = rMin->right_;
                }
                else 
                {
                    parent->right_ = rMin->right_;
                }
                delete pRelease;
                pRelease = NULL;
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

// 中序遍历非递归
// 左 -> 根 -> 右
void MidTraverIteration(TreeNode* pNode)
{
    std::vector<TreeNode*> stack;
    while (pNode || !stack.empty())
    {
        // 迭代某一棵子树(或左子树或右子树)
        while (pNode)
        {
            stack.push_back(pNode);
            pNode = pNode->left_;
        }
        if (!stack.empty())
        {
            TreeNode* pPop = stack.back();
            assert(pPop);
            stack.erase(stack.begin() + stack.size() - 1);
            printf("%d -> ", pPop->obj_);
            // 切换到右子树
            pNode = pPop->right_;
        }
    }
    printf("\n");
}

// 前序遍历非递归
// 根 -> 左 -> 右
void FirstTraverIteration(TreeNode* pNode)
{
    std::vector<TreeNode*> stack;
    stack.push_back(pNode);
    while (!stack.empty())
    {
        TreeNode *pPop = stack.back();
        assert(pPop);
        stack.erase(stack.begin() + stack.size() - 1);
        printf("%d -> ", pPop->obj_);

        if (pPop->right_)
        {
            stack.push_back(pPop->right_);
        }
        if (pPop->left_)
        {
            stack.push_back(pPop->left_);
        }
    }
    printf("\n");
}

// 后序遍历非递归
// 左 -> 右 -> 根
void LastTraverIteration(TreeNode* pNode)
{
    std::vector<TreeNode*> stack;
    TreeNode* preVisit = NULL;
    while (pNode || !stack.empty())
    {
        while (pNode)
        {
            stack.push_back(pNode);
            pNode = pNode->left_;
        }

        while (!stack.empty())
        {
            TreeNode* pPop = stack.back();
            assert(pPop);

            // 待删除节点不存在右子树或者右子树已经访问过
            // 删除待删除节点 更新上一次访问过的节点
            if (!pPop->right_ || pPop->right_ == preVisit)
            {
                // 确认可以弹出
                stack.erase(stack.begin() + stack.size() - 1);
                printf("%d -> ", pPop->obj_);
                preVisit = pPop;
            }
            // 存在右子树且右子树没有被访问过,切换到右子树
            // 保证右子树没访问之前不能访问根节点
            else 
            {
                pNode = pPop->right_;
                break;
            }
        }
    }
    printf("\n");
}

// 广度优先遍历
// 层/宽度遍历 -> 队列
void BFIteration(TreeNode* pNode)
{
    assert(pNode);
    TreeNode* pCur = NULL;
    std::list<TreeNode*> visited_queue;
    std::list<TreeNode*> unvisited_queue;
    unvisited_queue.push_back(pNode);
    while (!unvisited_queue.empty())
    {
        pCur = unvisited_queue.front();
        unvisited_queue.pop_front();
        assert(pCur);
        if (pCur->left_)
        {
            unvisited_queue.push_back(pCur->left_);
        }
        if (pCur->right_)
        {
            unvisited_queue.push_back(pCur->right_);
        }
        visited_queue.push_back(pCur);
        printf("%d -> ", pCur->obj_);
    }
    printf("\n");
}

// 深度优先遍历
// -> 栈
void DFIteration(TreeNode* pNode)
{
    assert(pNode);
    TreeNode* pCur = NULL;
    std::vector<TreeNode*> unvisited_stack;
    unvisited_stack.push_back(pNode);

    while (!unvisited_stack.empty())
    {
        pCur = unvisited_stack.back();
        unvisited_stack.erase(unvisited_stack.begin() + unvisited_stack.size() - 1);
        assert(pCur);
        if (pCur->right_)
        {
            unvisited_stack.push_back(pCur->right_);
        }
        if (pCur->left_)
        {
            unvisited_stack.push_back(pCur->left_);
        }
        printf("%d -> ", pCur->obj_);
    }
    printf("\n");
}

#define TOTAIL 1
#define TOHEAD 0
int Index(int data[], int start, int end, int val, int direction)
{
    if (direction == TOTAIL)
    {
        for (int idx = start; idx <= end; ++idx)
        {
            if (data[idx] == val)
            {
                return idx;
            }
        }
    }
    else if (direction == TOHEAD)
    {
        for (int idx = end; idx >= 0; --idx)
        {
            if (data[idx] == val)
            {
                return idx;
            }
        }
    }
    return -1;
}

// 根据树的前序和中序遍历序列建立二叉树
// first  [fstart, fend]
// middle [mstart, mend]
TreeNode* FMOrderBuildTree(int first[], int fstart, int fend, int middle[], int mstart, int mend)
{
    assert(first && middle);
    if (fstart > fend || mstart > mend)
    {
        return NULL;
    }
    
    TreeNode* pNode = new TreeNode(first[fstart]);
    assert(pNode);

    // 在中序序列中找到前序节点的值
    int idx = Index(middle, mstart, mend, first[fstart], TOTAIL);
    assert(idx != -1);

    // 依次将先序序列和中序序列分成左右子树,分别构建左右子树,限定边界条件
    // 遍历[fstart, fend]在[mstart, mend]中搜索
    pNode->left_ = FMOrderBuildTree(first, fstart+1, fstart+idx-mstart, middle, mstart, idx-1);
    pNode->right_ = FMOrderBuildTree(first, fstart+idx-mstart+1, fend, middle, idx+1, mend);
    return pNode;
}

// 根据树的中序和后续遍历序列建立二叉树
TreeNode* MLOrderBuikdTree(int middle[], int mstart, int mend, int last[], int lstart, int lend)
{
    assert(middle && last);
    if (mstart > mend || lstart > lend)
    {
        return NULL;
    }
    TreeNode* pNode = new TreeNode(last[lend]);
    assert(pNode);

    // 在中序序列中找出后续节点的值
    int idx = Index(middle, mstart, mend, last[lend], TOHEAD);
    assert(idx != -1);

    // 依次将中序序列和后序序列分成左右子树,分别构建左子树右子树,限定边界条件
    // 遍历lend->lstart在[mstart,mend]中搜索 
    pNode->left_ = MLOrderBuikdTree(middle, mstart, idx-1, last, lstart, lstart+idx-mstart-1);
    pNode->right_ = MLOrderBuikdTree(middle, idx+1, mend, last, lstart+idx-mstart, lend-1);
    return pNode;
}