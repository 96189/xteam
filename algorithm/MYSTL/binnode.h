#ifndef _BIN_NODE_H_
#define _BIN_NODE_H_

#include <cstddef>
#include <assert.h>
#include "stack.h"
#include "queue.h"

namespace MYSTL
{

// 节点位置
#define BinNodePosi(T) BinNode<T>*
// 节点高度(空树高度为-1)
#define stature(p) ((p) ? p->height : -1)
// 节点颜色
typedef enum { RB_RED, RB_BLACK } RBColor;

// BinNode状态与性质判断
#define IsRoot(x) (!((x)->parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x)->parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x)->parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x)->lChild)
#define HasRChild(x) ((x)->rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))

// 与BinNode具有特定关系的节点及指针
// 兄弟
#define sibling(p) (    \
    IsLChild(*(p)) ?    \
        (p)->parent->rChild :   \
        (p)->parent->lChild     \
)

// 叔叔
#define uncle(x) (      \
    IsLChild(*((x)->parent)) ?        \
        (x)->parent->parent->rChild : \
        (x)->parent->parent->lChild   \
)

// 来自父节点的指针
#define FromParentTo(x) (       \
    IsRoot(x) ? _root : (       \
        IsLChild(x) ? (x)->parent->lChild : (x)->parent->rChild \
    )                           \
)

// 二叉树节点模板类
template <typename T>
class BinNode 
{
public:
// 成员
    // 数值
    T data;
    // 父节点 左孩子 右孩子
    BinNodePosi(T) parent;
    BinNodePosi(T) lChild;
    BinNodePosi(T) rChild;
    // 高度
    int height;
    // Null Path Length 左式堆 也可直接用height代替
    int npl;
    // 颜色
    RBColor color;
// 构造函数
    BinNode()
        : parent(NULL), lChild(NULL), rChild(NULL), height(0), npl(1), color(RB_RED)
    {

    }
    BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
            int h = 0, int l = 1, RBColor c = RB_RED)
        : data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c)
    {

    }
// 操作接口
    // 统计以当前节点为根的子树的规模
    int size()
    {
        int size = 0;
        // 空树
        if (!this)
        {
            return 0;
        }
        // 叶子节点
        else if (IsLeaf(this))
        {
            return 1;
        }
        // 其他节点
        size += this->lChild->size();
        size += this->rChild->size();
        return size;
    }
    // 作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsLC(const T& e)
    {
        BinNodePosi(T) node = new BinNode(e, this);
        assert(node);
        this->lChild = node;
        return node;
    }
    // 作为当前节点的右孩子插入新节点
    BinNodePosi(T) insertAsRC(const T& e)
    {
        BinNodePosi(T) node = new BinNode(e, this);
        assert(node);
        this->rChild = node;
        return node;
    }
    // 取当前节点(非叶子节点)的直接后继(中序遍历意义下)
    // 当前节点右子树的最左端的节点
    BinNodePosi(T) succ()
    {
        assert(HasLChild(this));
        BinNodePosi(T) pCur = this->rChild;
        while (pCur->lChild)
        {
            pCur = pCur->lChild;
        }
        return pCur;
    }
    // 子树层次遍历
    template <typename VST>
    int travLevel(VST& vst)
    {
        int counter = 0;
        Queue<BinNodePosi(T)> q;
        BinNodePosi(T) pCur = NULL;
        q.enqueue(this);
        while (!q.empty())
        {
            pCur = q.dequeue();
            vst(pCur);
            ++counter;
            if (pCur->lChild)
            {
                q.enqueue(pCur->lChild);
            }
            if (pCur->rChild)
            {
                q.enqueue(pCur->rChild);
            }
        }
        return counter;
    }
    // 栈中存储左侧链 根据参数决定是否在遍历时打印
    template <typename VST>
    void goLeftChain(BinNodePosi(T) x, Stack<BinNodePosi(T)>& s, bool show, VST& vst)
    {
        while (x)
        {
            if (show)
            {
                vst(x);
            }
            s.push(x);
            x = x->lChild;
        }
    }
    // 子树深度遍历
    template <typename VST>
    int travDepth(VST& vst)
    {
        return 0;
    }
    // 子树先序遍历
    template <typename VST>
    int travPre(VST& vst)
    {
        // version 1
        // int counter = 0;
        // Stack<BinNodePosi(T)> s;
        // BinNodePosi(T) pCur = NULL;
        // s.push(this);
        // while (!s.empty())
        // {
        //     pCur = s.pop();
        //     vst(pCur);
        //     ++counter;
        //     // 记录沿途节点
        //     if (HasRChild(pCur))
        //     {
        //         s.push(pCur->rChild);
        //     }
        //     // 记录沿途节点
        //     if (HasLChild(pCur))
        //     {
        //         s.push(pCur->lChild);
        //     }
        //     // 向上回溯
        // }
        // return counter;

        // version 2
        {
            int counter = 0;
            BinNodePosi(T) pCur = this;
            Stack<BinNodePosi(T)> s;
            while (true)
            {
                // 先序遍历左侧链
                // 左侧链性质:对于某个节点 左子树为NULL或者左子树已访问过
                // 相当于访问根节点和左子树
                goLeftChain(pCur, s, true, vst);
                if (s.empty())
                    break;
                // 已访问根节点和左子树 下一次访问右子树
                pCur = s.pop()->rChild;
                ++counter;
            }
            return counter;
        }
    }
    // 子树中序遍历
    template <typename VST>
    int travIn(VST& vst)
    {
        // version 1
        int counter = 0;
        Stack<BinNodePosi(T)> s;
        s.push(this);
        BinNodePosi(T) pCur = NULL;
        while (!s.empty())
        {
            pCur = s.top();
            while (pCur->lChild)
            {
                s.push(pCur->lChild);
                pCur = pCur->lChild;
            }
            
            while (!s.empty())
            {
                // 由于栈的性质 左子树必定先于根节点访问
                pCur = s.pop();
                vst(pCur); 
                ++counter;  
                // 存在右子树 则处理右子树 始终保持右子树最后处理
                if (pCur->rChild)
                {
                    s.push(pCur->rChild);       
                    break;
                }
                // 否则 回溯将栈中缓存的数据弹出
            }
            // 以上保持 左子树 根节点 右子树的访问顺序
        }
        return counter;

        // version 2
        // {
        //     int counter = 0;
        //     BinNodePosi(T) pCur = this;
        //     Stack<BinNodePosi(T)> s;
        //     while (true)
        //     {
        //         // 左侧链 缓存根节点和左子树
        //         goLeftChain(pCur, s, false, vst);
        //         if (s.empty())
        //             break;
        //         // 左子树为空 或者已访问过
        //         // 访问根节点
        //         vst(s.top());
        //         ++counter;
        //         // 转向右子树
        //         pCur = s.pop()->rChild;
        //     }
        // }
        // return counter;
    }
    // 子树后续遍历
    template <typename VST>
    int travPost(VST& vst)
    {
        int counter = 0;
        Stack<BinNodePosi(T)> s;
        BinNodePosi(T) pCur = NULL;
        // 避免回溯时重复进入已经访问过的空间
        BinNodePosi(T) visited = NULL;
        s.push(this);
        while (!s.empty())
        {
            pCur = s.top();
            // 循环到此处必然时由于上一次切换到下一个右子树
            while (pCur->lChild)
            {
                s.push(pCur->lChild);
                pCur = pCur->lChild;
            }
            // 切换到右子树 需要1(根节点)次或者2次(根节点的左子树)
            // 跳过叶子节点和子树的根节点
            while (!s.empty())
            {
                pCur = s.top();
                // 确保右子树已经访问过
                // 在右子树已访问的前提下 访问左子树和根节点 由于栈的性质 左子树必定先于根节点访问
                // 不存在右子树 或者 右子树已访问过 
                if (!HasRChild(pCur) || pCur->rChild == visited)
                {
                    vst(pCur);
                    ++counter;
                    s.pop();
                    visited = pCur;
                }
                // 遇到右子树 则先访问右子树
                // 存在右子树 且 右子树未访问过
                else 
                {
                    s.push(pCur->rChild);
                    break;
                }
            }
            // 以上保证 左子树 右子树 根节点的访问顺序
        }
        return counter;
    }
// 判等器 比较器
    bool operator<(const BinNode& bn)
    {
        return this->data < bn.data;
    }
    bool operator==(const BinNode& bn)
    {
        return this->data == bn.data;
    }
};

}; // MYSTL

#endif
