#ifndef _LIST_H_
#define _LIST_H_
#include <cstddef>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
namespace MYSTL 
{

typedef int Rank;
#define ListNodePosi(T) ListNode<T>*

template <typename T>
class ListNode 
{
public:
    T data;
    // 前驱
    ListNodePosi(T) pred;
    // 后继
    ListNodePosi(T) succ;

    // 构造函数 针对header和trailer的构造
    ListNode() 
    {
        data = T();
        pred = succ = NULL;
    }
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {}

// 操作接口
    // 读取数据
    T& getData() 
    {
        return data;
    } 
    void setData(T val)
    {
        data = val;
    }
    // 在当前节点之前插入
    ListNodePosi(T) insertAsPred(const T& e)
    {
        ListNodePosi(T) pCur = new ListNode(e, this->pred, this);
        if (this->pred)
        {
            this->pred->succ = pCur;
        }
        this->pred = pCur;
        return pCur;
    }
    // 在当前节点之后插入
    ListNodePosi(T) insertAsSucc(const T& e)
    {
        ListNodePosi(T) pCur = new ListNode(e, this, this->succ);
        if (this->succ)
        {
            this->succ->pred = pCur;
        }
        this->succ = pCur;
        return pCur;
    }
};


template <typename T>
void swap(ListNodePosi(T) first, ListNodePosi(T) last)
{
    T val = first->getData();
    first->setData(last->getData());
    last->setData(val);
}

// 双向链表定义
// 双向链表结束不以NULL为标记
// 尾端存在trailer哨兵节点
template <typename T>
class List 
{
private:
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;
protected:
    // 创建列表时的初始化
    void init()
    {
        header = new ListNode<T>();
        // trailer = new ListNode<T>();
        trailer = header->insertAsSucc(T());
        _size = 0;
    }
    // 清除所有节点
    int clear()
    {
        int count = 0;
        for (ListNodePosi(T) pCur = header->succ;
             pCur != trailer;
             pCur = pCur->succ)
        {
            delete pCur;
            ++count;
        }
        assert(count == _size);
        delete header;
        header = NULL;
        delete trailer;
        trailer = NULL;
        return count;
    }
    // 复制列表中自位置P起的n项
    void copyNodes(ListNodePosi(T) p, int n)
    {
        ListNodePosi(T) pCur = p->pred;
        while ((pCur = pCur->succ) != trailer && n--)
        {
            this->insertAsLast(pCur->getData());
        }
    }
    // 有序列表区间归并
    void merge(ListNodePosi(T)& , int , List<T>& ,ListNodePosi(T), int )
    {

    }
    // 对从p开始的连续n个节点归并排序
    void mergeSort(ListNodePosi(T)& , int)
    {

    }
    // 对从p开始的连续n个节点选择排序
    void selectionSort(ListNodePosi(T) p, int n)
    {
        int count = n;
        ListNodePosi(T) ofirst = p;
        while (--count)
        {
            ofirst = ofirst->succ;
        }
        for (ListNodePosi(T) ulast = ofirst; /* 初始状态有序空间为0 无序空间占据整个空间 */
             ulast != p;                     /* 有序空间未填充整个无序空间 */
             ulast = ulast->pred)            /* 每次循环有序空间增长 无序空间减小 */
        {
            // 在[ufirst, ulast]区间中找最大值
            ListNodePosi(T) pMax = p;
            for (ListNodePosi(T) ufirst = p;
                 ufirst != ulast->succ;
                 ufirst = ufirst->succ)
            {
                if (ufirst->getData() >= pMax->getData())
                {
                    pMax = ufirst;
                }
            }
            // 同一位置不交换(已在有序空间)
            if (pMax != ulast)
            {
                swap(pMax, ulast);
            }
        }
    }
    // 对从p开始的连续n个节点插入排序
    void insertionSort(ListNodePosi(T) p, int n)
    {
        ListNodePosi(T) olast = p;
        ListNodePosi(T) ufirst = olast->succ;
        int oCount = 1;
        ListNodePosi(T) pos = NULL;
        ListNodePosi(T) del = NULL;
        while (ufirst != trailer)
        {
            pos = search(ufirst->getData(), oCount, olast);
            // 在有序区间找到待插入元素的插入位置
            if (pos != olast)
            {
                // 有序空间增长
                insertAsSucc(pos, ufirst->getData());
                // 无序空间减小
                del = ufirst;
                ufirst = ufirst->succ;
                // 删除已排序插入的节点
                remove(del);
            }
            // 待插入元素和有序空间最后一个元素已经相邻 无需做任何操作
            else
            {
                // 无序空间减小
                ufirst = ufirst->succ;
                // 有序空间增长
                olast = olast->succ;
            }
            //
            ++oCount;
        }
    }
public:
// 构造函数
    // 默认
    List()
    {
        init();
    }
    // 整体复制列表L
    List(const List<T>& L)
    {
        init();
        copyNodes(L.first(), L.size());
    }
    // 复制列表L中第r项起的n项
    List(const List<T>& L, Rank r, Rank n)
    {
        init();
        ListNodePosi(T) pCur = L.first();
        while (r-- > 0)
        {
            pCur = pCur->succ;
        }
        copyNodes(pCur, n - r - 1);
    }
    // 复制列表中自位置p起的n项
    List(ListNodePosi(T) p, int n)
    {
        init();
        copyNodes(p, n);
    }
    // 析构函数
    // 释放包含头尾哨兵在内的所有节点
    ~List()
    {
        clear();
    }
    // 只读访问接口
    // 规模
    int size() const
    {
        return _size;
    }
    // 判空
    bool empty() const
    {
        return _size == 0;
    }
    // 重载[]支持寻秩访问
    T& operator[](Rank r) const
    {
        assert(r < _size);
        ListNodePosi(T) pCur = header->succ;
        while (r-- > 0)
        {
            pCur = pCur->succ;
        }
        return pCur->getData();
    }
    // 首节点位置
    ListNodePosi(T) first() const
    {
        return header->succ;
    }
    // 末节点位置
    ListNodePosi(T) last() const
    {
        return trailer->pred;
    }
    // 判断位置p是否对外合法
    bool valid(ListNodePosi(T) p) const
    {
        return (p && p != header && p != trailer);
    }
    // 判断列表是否已经有序
    int disordered() const
    {
        int count = 0;
        for (ListNodePosi(T) pCur = header->succ;
             pCur->succ != trailer;
             pCur = pCur->succ)
        {
            count += pCur->getData() > pCur->succ->getData() ? 1 : 0;
        }
        return count;
    }
    // 无序列表查找
    ListNodePosi(T) find(const T& e) const
    {
         return find(e, _size, last());
    }
    // 无序区间查找
    ListNodePosi(T) find(const T& e, int n, ListNodePosi(T) p) const
    {
        assert(n > 0 && p);
        ListNodePosi(T) pCur = p->succ;
        while ((pCur = pCur->pred) != trailer && --n)
        {
            if (pCur->getData() == e)
                break;
        }
        if (n == 0 && pCur->getData() != e)
        {
            return NULL;
        }
        return pCur;
    }
    // 有序列表查找
    ListNodePosi(T) search(const T& e)
    {
        return search(e, _size, last());
    }
    // 有序区间查找
    ListNodePosi(T) search(const T& e, int n, ListNodePosi(T) p) const
    {
        assert(n > 0 && p);
        ListNodePosi(T) pCur = p->succ;
        while ((pCur = pCur->pred) != trailer && --n)
        {
            if (pCur->getData() <= e)
                break;
        }
        return pCur;
    }
    // 在p及其n-1个后继中选出最大者
    T selectMax(ListNodePosi(T) p, int n)
    {
        ListNodePosi(T) pCur = p;
        T maxVal = pCur->getData();
        while ((pCur = pCur->succ) != trailer && --n)
        {
            if (pCur->getData() > maxVal)
            {
                maxVal = pCur->getData();
            }
        }
        return maxVal;
    }
    // 整体最大者
    T selectMax()
    {
        return selectMax(first(), _size);
    }
// 可写访问接口
    // 将e当作首节点插入
    void insertAsFirst(const T& e)
    {
        first()->insertAsPred(e);
        ++_size;
    }
    // 将e当作末节点插入
    void insertAsLast(const T& e)
    {
        last()->insertAsSucc(e);
        ++_size;
    }
    // 将e当作p的前驱插入
    void insertAsPred(ListNodePosi(T) p, const T& e)
    {
        p->insertAsPred(e);
        ++_size;
    }
    // 将e当作p的后继插入
    void insertAsSucc(ListNodePosi(T) p, const T& e)
    {
        p->insertAsSucc(e);
        ++_size;
    }
    // 删除合法位置p处的节点 返回被删除的节点
    T remove(ListNodePosi(T) p)
    {
        assert(p != header && p != trailer);
        T val = p->getData();
        ListNodePosi(T) prev = p->pred;
        ListNodePosi(T) next = p->succ;
        prev->succ = next;
        next->pred = prev;
        delete p;
        p = NULL;
        --_size;
        return val;
    }
    // 全列表归并
    void merge(List<T>& L)
    {

    }
    // 列表区间排序
    void sort(ListNodePosi(T) s, int n)
    {
        selectionSort(s, n);
    }
    // 列表整体排序
    void sort()
    {
        sort(header->succ, _size);
    }
    // 无序去重
    int deduplicate()
    {
        // 有序区间的最后一个节点的指针
        ListNodePosi(T) olast = first();
        // 无序区间第一个节点节点的指针
        ListNodePosi(T) ufirst = olast->succ;
        ListNodePosi(T) pos = NULL;
        ListNodePosi(T) del = NULL;
        int oCount = 1;
        int delCount = 0;
        while (ufirst != trailer)
        {
            pos = find(ufirst->getData(), oCount, olast);
            if (pos)
            {
                // 有序区间存在与无序区间重复的值
                del = ufirst;
            }
            ufirst = ufirst->succ;
            if (del)
            {
                // 删除无序区间中重复的值
                ++delCount;
                remove(del);
            }
            else
            {
                // 有序区间增长
                ++oCount;
                olast = olast->succ;
            }
            del = NULL;
        }
        return delCount;
    }
    // 有序去重
    int uniquify()
    {
        return 0;
    }
    // 前后逆置
    void reverse()
    {
        ListNodePosi(T) f = first();
        ListNodePosi(T) l = last();
        while (f != l && f->pred != l)
        {
            swap(f, l);
            f = f->succ;
            l = l->pred;
        }
    }
    // 遍历
    // 函数指针遍历
    void traverse(void (*visit)(T& val))
    {
        for (ListNodePosi(T) pCur = header->succ;
             pCur != trailer;
             pCur = pCur->succ)
        {
            visit(pCur->getData());
        }
    }
    // 函数对象遍历
    template <typename VST>
    void traverse(VST& funobj);
}; // List

}; // namespace MYSTL

#endif