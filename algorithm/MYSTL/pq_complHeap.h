#ifndef _PQ_COMPL_HEAP_H_
#define _PQ_COMPL_HEAP_H_

#include "priorityQueue.h"
#include "vector.h"

namespace MYSTL 
{

// 判断PQ[i]是否是合法节点
#define InHeap(n, i) (((-1) < (i)) && ((i) < (n)))
// PQ[i]的父节点
#define Parent(i) ((i-1) >> 1)
// 最后一个内部节点(末节点的父亲)
#define LastInternal(n) Parent(n-1)
// PQ[i]的左孩子
#define LChild(i) (1 + ((i) << 1))
// PQ[i]的右孩子
#define RChild(i) ((1 + (i)) << 1)
// 判断PQ[i]是否有父亲
#define ParentValid(i) (0 < i)
// 判断PQ[i]是否有一个左孩子
#define LChildValid(n, i) InHeap(n, LChild(i))
// 判断PQ[i]是否有两个孩子
#define RchildValid(n, i) InHeap(n, RChild(i))
// 取较大的(相等则前者优先)
#define Bigger(PQ, i, j) (((PQ[i]) < (PQ[j])) ? j : i)
// 父子三者中最大的 (相等时父节点优先)
#define ProperParent(PQ, n, i) \
        (RchildValid(n, i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)),RChild(i)) : \
        (LChildValid(n, i) ? Bigger(PQ, i, LChild(i)) : i \
        ) \
)

// 基于向量 以完全二叉堆形式实现的PQ
template <typename T>
class PQ_ComplHeap : public PQ<T>, public Vector<T> 
{
protected:
    // 逻辑上的结构为完全二叉树
    Rank percolateDown(Rank n, Rank i) // 下滤
    {
        Rank j; // i及其孩子中最大的
        while (i != (j = ProperParent(this->_elem, n, i)))
        {
            swap(this->_elem[i], this->_elem[j]);
            i = j;
        }
        return i;
    }
    // 逻辑上的结构为完全二叉树
    Rank percolateUp(Rank i) // 上滤
    {
        while (ParentValid(i))
        {
            Rank j = Parent(i);
            // 孩子节点值小于 父节点值
            if (this->_elem[i] < this->_elem[j]) break;
            // 否则 交换后向上继续判断
            swap(this->_elem[i], this->_elem[j]);
            i = j;
        }
        // 返回上滤最终抵达的位置
        return i;
    }
    void heapify(Rank n) // floyd建堆算法
    {
        for (int i = this->_size / 2 - 1; i >= 0; --i)
        {
            percolateDown(n,i);
        }
    }
public:
    PQ_ComplHeap() { } // 默认构造
    PQ_ComplHeap(T* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); heapify(hi - lo); } // 批量构造
    PQ_ComplHeap(T* A, Rank n) { copyFrom(A, 0, n); heapify(n); } // 批量构造
// 接口实现
    // 将词条插入完全二叉堆中
    void insert(T e)
    {
        this->insert(e); // 在向量末尾插入
        percolateUp(this->_size - 1); // 上滤调整
    }
    // 获取非空完全二叉堆中优先级最高的词条
    T getMax()
    {
        return this->_elem[0];
    }
    T delMax()
    {
        T maxElem = this->_elem[0];   // 备份堆顶元素
        this->_elem[0] = this->remove(this->_size - 1);   // 以堆末尾元素替换堆顶(删除堆末尾元素)
        percolateDown(this->_size, 0);    // 对新堆顶实施下滤调整
        return maxElem;
    }
}; // PQ_ComplHeap

}; // MYSTL

#endif 