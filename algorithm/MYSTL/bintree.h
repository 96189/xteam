#ifndef _BIN_TREE_H_
#define _BIN_TREE_H_
#include "binnode.h"
#include <algorithm>
#include <assert.h>
namespace MYSTL
{

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
            assert(remove(_root) == _size);
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
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& t)
    {
        return NULL;
    }
    // t作为右子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& t)
    {
        return NULL;
    }
    // 删除以位置x处节点为根的子树 返回该子树原先的规模
    int remove(BinNodePosi(T) x)
    {
        return 0;
    }
    // 将子树x从当前树中摘除 并将其转换为一颗独立的子树
    BinTree<T>* secede(BinNodePosi(T) x)
    {
        return NULL;
    }
    // 层次遍历
    template <typename VST>
    void travLevel(VST& visit)
    {
        if (_root)
            _root->travLevel(visit);
    }
    // 深度遍历
    template <typename VST>
    void travDepth(VST& visit)
    {
        if (_root)
            _root->travDepth(visit);
    }
    // 先序遍历
    template <typename VST>
    void travPre(VST& visit)
    {
        if (_root)
            _root->travPre(visit);
    }
    // 中序遍历
    template <typename VST>
    void travIn(VST& visit)
    {
        if (_root)
            _root->travIn(visit);
    }
    // 后续遍历
    template <typename VST>
    void travPost(VST& visit)
    {
        if (_root)
            _root->travPost(visit);
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
    int findInOrder(const T& val, const T* i, int istart, int iend)
    {
        while (istart <= iend)
        {
            if (i[istart] == val) return istart;
            ++istart;
        }
        return -1;
    }
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
        if (pstart <= pend)
        {
            index = findInOrder(p[pstart], i, istart, iend);
            root->data = i[index];
            root->lChild = buildTreePreIn(p, pstart + 1, pstart+(index-istart), i, istart, index-1);
            root->rChild = buildTreePreIn(p, pstart+(index-istart)+1, pend, i, index+1, iend);
            ++pstart;
        }
        return root;
    }
    // 后续和中序序列建树
    // 参数为前序序列区间和中序序列区间 返回树根
    BinNodePosi(T) buildTreePosIn(const T* p, int pstart, int pend, const T* i, int istart, int iend)
    {
        return NULL;
    }
    void buildTreePreIn(const T* p, int plen, const T* i, int ilen)
    {
        _root = buildTreePreIn(p, 0, plen-1, i, 0, ilen-1);
    }
    void buildTreePosIn(const T* p, int plen, const T* i, int ilen)
    {
        _root = buildTreePosIn(p, 0, plen-1, i, 0, ilen-1);
    }
};


}; // MYSTL

#endif