#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_
#include "binnode.h"
#include <algorithm>
#include <assert.h>
namespace MYSTL
{

template <typename T>
class ReleaseNode 
{
public:
    void operator()(BinNodePosi(T) p)
    {
        // printf("delete %p\n", p);
        delete p;
        p = NULL;
    }
};

template <typename T>
class BinTree 
{
protected:
// 
    // 规模
    int _size;
    // 根节点
    BinNodePosi(T) _root;

// function
    // 来自父节点的引用
    BinNodePosi(T)& FromParentTo(BinNodePosi(T)& x)
    {
        return IsRoot(x) ? _root : IsLChild(x) ? (x)->parent->lChild : (x)->parent->rChild;
    }
    BinNodePosi(T) tallerChild(BinNodePosi(T) x) 
    {
        BinNodePosi(T) p = NULL;
        /* 左子树高 左子树优先 */
        if (stature((x)->lChild) > stature((x)->rChild))
        {
            p = (x)->lChild;
        }
        /* 右子树高 右子树优先 */
        else if (stature((x)->lChild) < stature((x)->rChild))
        {
            p = (x)->rChild;
        }
        /* 左右子树同高 与父节点同侧者优先 */
        else
        {
            p = IsLChild(x) ? (x)->lChild : (x)->rChild;
        }
        return p;
    }
    // 更新节点x的高度
    virtual int updateHeight(BinNodePosi(T) x)
    {
        if (!x)
        {
            // 空树高度为-1
            return -1;
        }
        x->height = std::max<T>(updateHeight(x->lChild), updateHeight(x->rChild)) + 1;
        return x->height;
    }
    // 更新节点x及其祖先的高度
    void updateHeightAbove(BinNodePosi(T) x)
    {
        while (x)
        {
            updateHeight(x);
            x = x->parent;
        }
    }
public:
    // 构造函数
    BinTree() : _size(0), _root(NULL) { }
    // 析构函数
    ~BinTree()
    {
        if (0 < _size)
        {
            int n = remove(_root);
            assert(n == _size);
        }
    }
    // 规模
    int size() const
    {
        return _size;
    } 
    // 判空
    bool empty() const 
    {
        return !_root;
    }
    // 树根
    BinNodePosi(T) root() const 
    {
        return _root;
    }
    // 插入根节点
    BinNodePosi(T) insertAsRoot(const T& e)
    {
        ++_size;
        return _root = new BinNode<T>(e);
    }
    // 作为x的左孩子插入
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x, const T& e)
    {
        x->lChild = new BinNode<T>(e, x);
        updateHeightAbove(x);
        ++_size;
        return x->lChild;
    }
    // 作为x的右孩子插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x, const T& e)
    {
        x->rChild = new BinNode<T>(e, x);
        updateHeightAbove(x);
        ++_size;
        return x->rChild;
    }
    // t作为左子树接入
    void attachAsLC(BinNodePosi(T) x, BinNodePosi(T) t)
    {
        x->lChild = t;
        if (t) t->parent = x;
    }
    // t作为右子树接入
    void attachAsRC(BinNodePosi(T) x, BinNodePosi(T) t)
    {
        x->rChild = t;
        if (t) t->parent = x;
    }
    // 删除以位置x处节点为根的子树 返回实际删除的节点个数
    int remove(BinNodePosi(T) x)
    {
        // 遍历释放节点
        ReleaseNode<T> rn;
        // 删除不能调用travPre() 此函数左侧链遍历时会删除节点 后序流程无法继续
        // 会造成对一个地址两次释放
        return x->travLevel(rn);
    }
    // 将子树x从当前树中摘除 并将其转换为一颗独立的子树
    BinTree<T>* secede(BinNodePosi(T) x)
    {
        return NULL;
    }
    // 层次遍历
    template <typename VST>
    int travLevel(VST& visit)
    {
        if (_root)
            return _root->travLevel(visit);
        return 0;
    }
    // 深度遍历
    template <typename VST>
    int travDepth(VST& visit)
    {
        if (_root)
            _root->travDepth(visit);
        return 0;
    }
    // 先序遍历
    template <typename VST>
    int travPre(VST& visit)
    {
        if (_root)
            return _root->travPre(visit);
        return 0;
    }
    // 中序遍历
    template <typename VST>
    int travIn(VST& visit)
    {
        if (_root)
            return _root->travIn(visit);
        return 0;
    }
    // 后续遍历
    template <typename VST>
    int travPost(VST& visit)
    {
        if (_root)
            return _root->travPost(visit);
        return 0;
    }
// 比较器 判等器
    bool operator<(const BinTree<T>& t)
    {
        return _root && t._root && lt(_root, t._root);
    }
    bool operator==(const BinTree<T> t)
    {
        return _root && t._root && (_root == t._root);
    }
// 建树
    // 前序和中序序列建树
    // 参数为前序序列区间和中序序列区间 返回树根
    BinNodePosi(T) buildTreePreIn(const T* p, int pstart, int pend, const T* i, int istart, int iend)
    {
        if (pend < pstart || iend < istart)
        {
            return NULL;
        }
        int index = -1;
        BinNodePosi(T) root = new BinNode<T>();
        // 在中序序列i中查找前序序列中的p[pstart]项 返回该项在中序序列中的位置index
        for (int index = istart; index <= iend; ++index)
        {
            if (p[pstart] == i[index])
            {
                root->data = i[index];
                ++_size;
                // 分解问题规模
                root->lChild = buildTreePreIn(p, pstart + 1, pstart + (index - istart), i, istart, index - 1);
                // 分解问题规模
                root->rChild = buildTreePreIn(p, pstart + (index - istart) + 1, pend, i, index + 1, iend);
                // 找到提前结束循环
                break;
            }
        }
        // TODO 判断若一次未查找到 则两个序列无法建树 应提前返回
        return root;
    }
    // 后续和中序序列建树
    // 参数为前序序列区间和中序序列区间 返回树根
    BinNodePosi(T) buildTreePosIn(const T* p, int pstart, int pend, const T* i, int istart, int iend)
    {
        if (pend < pstart || iend < istart)
        {
            return NULL;
        }
        BinNodePosi(T) root = new BinNode<T>();
        BinNodePosi(T) lChild = NULL;
        BinNodePosi(T) rChild = NULL;
        // 在中序序列i中查找后序序列的p[end]项 查找得到中序序列中的下标idx
        for (int idx = istart; idx <= iend; ++idx)
        {
            if (p[pend] == i[idx])
            {
                root->data = i[idx];
                ++_size;
                // 分解问题规模
                lChild = buildTreePosIn(p, pstart, pstart+(idx-istart)-1, i, istart, idx-1);
                root->lChild = lChild;
                if (lChild) lChild->parent = root;
                // 分解问题规模
                rChild = buildTreePosIn(p, pstart+(idx-istart), pend-1, i, idx+1, iend);
                root->rChild = rChild;
                if (rChild) rChild->parent = root;
                //  找到 提前结束循环
                break;
            }
        }
        // TODO 判断若一次未查找到 则两个序列无法建树 应提前返回
        return root;
    }
    void buildTreePreIn(const T* p, int plen, const T* i, int ilen)
    {
        _root = buildTreePreIn(p, 0, plen-1, i, 0, ilen-1);
        updateHeight(_root);
    }
    void buildTreePosIn(const T* p, int plen, const T* i, int ilen)
    {
        _root = buildTreePosIn(p, 0, plen-1, i, 0, ilen-1);
        updateHeight(_root);
    }
// 搜索
    BinNodePosi(T) &searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot)
    {
        if (!v || v->data == e) return v;
        // hot指向最终匹配节点的父节点 若树中无匹配节点则指向搜素中最后访问的节点
        hot = v;
        return searchIn((e < v->data ? v->lChild : v->rChild), e, hot);
    }
// 删除
    // 删除位置x的节点 x由查找算法确定 调用者确保x不为NULL
    // 返回值指向实际被删除节点的接替者 hot指向实际被删除节点的父节点
    BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot)
    {
        // 实际被删除的节点
        BinNodePosi(T) w = x;
        // 实际被删除节点的接替者
        BinNodePosi(T) succ = NULL;
        // 不存在左子树(考虑极端情况退化为链表)
        if (!HasLChild(x))
        {
            // 断链 x引用类型参数 
            succ = x = x->rChild;
        } 
        // 不存在右子树(考虑极端情况退化为链表)
        else if (!HasRChild(x))
        {
            // 断链 x引用类型参数
            succ = x = x->lChild;
        }       
        // 左子树右子树都存在(考虑删除的是根节点的情况)
        else 
        {
            // w中序遍历意义下的直接后继
            w = w->succ();
            // 转移待删除数据
            swap(x->data, w->data);
            // 实际待删除数据的父节点
            BinNodePosi(T) u = w->parent;
            assert(u != NULL);
            // u == x 则w是x的右孩子
            // u != x 则w是u的孩子 u是x的子孙节点
            // 断链
            ((u == x) ? u->rChild : u->lChild) = succ = w->rChild;
        }
        // 后继不为空 维护后继的父指针
        hot = w->parent;
        if (succ) succ->parent = hot;
        delete w;
        return succ;
    }
};


}; // MYSTL

#endif