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
    // 取当前节点的直接后继(中序遍历意义下)
    BinNodePosi(T) succ()
    {
        return NULL;
    }
    // 子树层次遍历
    template <typename VST>
    void travLevel(VST& vst)
    {
        Queue<BinNodePosi(T)> q;
        BinNodePosi(T) pCur = NULL;
        q.enqueue(this);
        while (!q.empty())
        {
            pCur = q.dequeue();
            vst(pCur->data);
            if (pCur->lChild)
            {
                q.enqueue(pCur->lChild);
            }
            if (pCur->rChild)
            {
                q.enqueue(pCur->rChild);
            }
        }
    }
    // 栈中存储左侧链 根据参数决定是否在遍历时打印
    template <typename VST>
    void goLeftChain(BinNodePosi(T) x, Stack<BinNodePosi(T)>& s, bool show, VST& vst)
    {
        while (x)
        {
            if (show)
            {
                vst(x->data);
            }
            s.push(x);
            x = x->lChild;
        }
    }
    // 子树深度遍历
    template <typename VST>
    void travDepth(VST& vst)
    {
        return;
    }
    // 子树先序遍历
    template <typename VST>
    void travPre(VST& vst)
    {
        // version 1
        // Stack<BinNodePosi(T)> s;
        // BinNodePosi(T) pCur = NULL;
        // s.push(this);
        // while (!s.empty())
        // {
        //     pCur = s.pop();
        //     vst(pCur->data);
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

        // version 2
        {
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
            }
        }
    }
    // 子树中序遍历
    template <typename VST>
    void travIn(VST& vst)
    {
        // version
        // Stack<BinNodePosi(T)> s;
        // s.push(this);
        // BinNodePosi(T) pCur = NULL;
        // BinNodePosi(T) visited = NULL;
        // while (!s.empty())
        // {
        //     pCur = s.top();
        //     // 沿左侧链 到达左侧链起始 记录沿途节点
        //     while (!visited && HasLChild(pCur))
        //     {
        //         pCur = pCur->lChild;
        //         s.push(pCur);
        //     }
            
        //     // 左子树为空 
        //     // 弹出当前子树根节点
        //     pCur = s.pop();
        //     vst(pCur->data);
        //     visited = pCur;
        //     // 当前子树是否存在右子树
        //     if (pCur->rChild)
        //     {
        //         s.push(pCur->rChild);
        //         // 新子树的左边尚未访问
        //         visited = NULL;
        //     }

        //     // 向上回溯
        // }
        // version 2
        {
            BinNodePosi(T) pCur = this;
            Stack<BinNodePosi(T)> s;
            while (true)
            {
                // 左侧链 缓存根节点和左子树
                goLeftChain(pCur, s, false, vst);
                if (s.empty())
                    break;
                // 左子树为空 或者已访问过
                // 访问根节点
                vst(s.top()->data);
                // 转向右子树
                pCur = s.pop()->rChild;
            }
        }
    }
    // 子树后续遍历
    template <typename VST>
    void travPost(VST& vst)
    {

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
