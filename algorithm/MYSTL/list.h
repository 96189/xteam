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
    T val_;
    // 前驱
    ListNodePosi(T) pred_;
    // 后继
    ListNodePosi(T) succ_;

    // 构造函数 针对header_和trailer_的构造
    ListNode() 
    {
        val_ = T();
        pred_ = succ_ = NULL;
    }
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : val_(e), pred_(p), succ_(s) {}

// 操作接口
    // 读取数据
    T& getData() 
    {
        return val_;
    } 
    void setData(T val)
    {
        val_ = val;
    }
    // 在当前节点之前插入
    ListNodePosi(T) insertAsPred(const T& e)
    {
        ListNodePosi(T) pCur = new ListNode(e, this->pred_, this);
        if (this->pred_)
        {
            this->pred_->succ_ = pCur;
        }
        this->pred_ = pCur;
        return pCur;
    }
    // 在当前节点之后插入
    ListNodePosi(T) insertAsSucc(const T& e)
    {
        ListNodePosi(T) pCur = new ListNode(e, this, this->succ_);
        if (this->succ_)
        {
            this->succ_->pred_ = pCur;
        }
        this->succ_ = pCur;
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
// 尾端存在trailer_哨兵节点
template <typename T>
class List 
{
private:
    int size_;
    ListNodePosi(T) header_;
    ListNodePosi(T) trailer_;
protected:
    // 创建列表时的初始化
    void init()
    {
        header_ = new ListNode<T>();
        // trailer_ = new ListNode<T>();
        trailer_ = header_->insertAsSucc(T());
        size_ = 0;
    }
    // 清除所有节点
    int clear()
    {
        int count = 0;
        for (ListNodePosi(T) pCur = header_->succ_;
             pCur != trailer_;
             pCur = pCur->succ_)
        {
            delete pCur;
            ++count;
        }
        assert(count == size_);
        delete header_;
        header_ = NULL;
        delete trailer_;
        trailer_ = NULL;
        return count;
    }
    // 复制列表中自位置P起的n项
    void copyNodes(ListNodePosi(T) p, int n)
    {
        ListNodePosi(T) pCur = p->pred_;
        while ((pCur = pCur->succ_) != trailer_ && n--)
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
            ofirst = ofirst->succ_;
        }
        for (ListNodePosi(T) ulast = ofirst; /* 初始状态有序空间为0 无序空间占据整个空间 */
             ulast != p;                     /* 有序空间未填充整个无序空间 */
             ulast = ulast->pred_)            /* 每次循环有序空间增长 无序空间减小 */
        {
            // 在[ufirst, ulast]区间中找最大值
            ListNodePosi(T) pMax = p;
            for (ListNodePosi(T) ufirst = p;
                 ufirst != ulast->succ_;
                 ufirst = ufirst->succ_)
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
        ListNodePosi(T) ufirst = olast->succ_;
        int oCount = 1;
        ListNodePosi(T) pos = NULL;
        ListNodePosi(T) del = NULL;
        while (ufirst != trailer_)
        {
            pos = search(ufirst->getData(), oCount, olast);
            // 在有序区间找到待插入元素的插入位置
            if (pos != olast)
            {
                // 有序空间增长
                insertAsSucc(pos, ufirst->getData());
                // 无序空间减小
                del = ufirst;
                ufirst = ufirst->succ_;
                // 删除已排序插入的节点
                remove(del);
            }
            // 待插入元素和有序空间最后一个元素已经相邻 无需做任何操作
            else
            {
                // 无序空间减小
                ufirst = ufirst->succ_;
                // 有序空间增长
                olast = olast->succ_;
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
            pCur = pCur->succ_;
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
        return size_;
    }
    // 判空
    bool empty() const
    {
        return size_ == 0;
    }
    // 重载[]支持寻秩访问
    T& operator[](Rank r) const
    {
        assert(r < size_);
        ListNodePosi(T) pCur = header_->succ_;
        while (r-- > 0)
        {
            pCur = pCur->succ_;
        }
        return pCur->getData();
    }
    // 首节点位置
    ListNodePosi(T) first() const
    {
        return header_->succ_;
    }
    // 末节点位置
    ListNodePosi(T) last() const
    {
        return trailer_->pred_;
    }
    // 判断位置p是否对外合法
    bool valid(ListNodePosi(T) p) const
    {
        return (p && p != header_ && p != trailer_);
    }
    // 判断列表是否已经有序
    int disordered() const
    {
        int count = 0;
        for (ListNodePosi(T) pCur = header_->succ_;
             pCur->succ_ != trailer_;
             pCur = pCur->succ_)
        {
            count += pCur->getData() > pCur->succ_->getData() ? 1 : 0;
        }
        return count;
    }
    // 无序列表查找
    ListNodePosi(T) find(const T& e) const
    {
         return find(e, size_, last());
    }
    // 无序区间查找
    ListNodePosi(T) find(const T& e, int n, ListNodePosi(T) p) const
    {
        assert(n > 0 && p);
        ListNodePosi(T) pCur = p->succ_;
        while ((pCur = pCur->pred_) != trailer_ && --n)
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
        return search(e, size_, last());
    }
    // 有序区间查找
    ListNodePosi(T) search(const T& e, int n, ListNodePosi(T) p) const
    {
        assert(n > 0 && p);
        ListNodePosi(T) pCur = p->succ_;
        while ((pCur = pCur->pred_) != trailer_ && --n)
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
        while ((pCur = pCur->succ_) != trailer_ && --n)
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
        return selectMax(first(), size_);
    }
// 可写访问接口
    // 将e当作首节点插入
    void insertAsFirst(const T& e)
    {
        first()->insertAsPred(e);
        ++size_;
    }
    // 将e当作末节点插入
    void insertAsLast(const T& e)
    {
        last()->insertAsSucc(e);
        ++size_;
    }
    // 将e当作p的前驱插入
    void insertAsPred(ListNodePosi(T) p, const T& e)
    {
        p->insertAsPred(e);
        ++size_;
    }
    // 将e当作p的后继插入
    void insertAsSucc(ListNodePosi(T) p, const T& e)
    {
        p->insertAsSucc(e);
        ++size_;
    }
    // 删除合法位置p处的节点 返回被删除的节点
    T remove(ListNodePosi(T) p)
    {
        assert(p != header_ && p != trailer_);
        T val = p->getData();
        ListNodePosi(T) prev = p->pred_;
        ListNodePosi(T) next = p->succ_;
        prev->succ_ = next;
        next->pred_ = prev;
        delete p;
        p = NULL;
        --size_;
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
        sort(header_->succ_, size_);
    }
    // 无序去重
    int deduplicate()
    {
        // 有序区间的最后一个节点的指针
        ListNodePosi(T) olast = first();
        // 无序区间第一个节点节点的指针
        ListNodePosi(T) ufirst = olast->succ_;
        ListNodePosi(T) pos = NULL;
        ListNodePosi(T) del = NULL;
        int oCount = 1;
        int delCount = 0;
        while (ufirst != trailer_)
        {
            pos = find(ufirst->getData(), oCount, olast);
            if (pos)
            {
                // 有序区间存在与无序区间重复的值
                del = ufirst;
            }
            ufirst = ufirst->succ_;
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
                olast = olast->succ_;
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
        while (f != l && f->pred_ != l)
        {
            swap(f, l);
            f = f->succ_;
            l = l->pred_;
        }
    }
    // 遍历
    // 函数指针遍历
    void traverse(void (*visit)(T& val))
    {
        for (ListNodePosi(T) pCur = header_->succ_;
             pCur != trailer_;
             pCur = pCur->succ_)
        {
            visit(pCur->getData());
        }
    }
    // 函数对象遍历
    template <typename VST>
    void traverse(VST& funobj);

    // https://juejin.im/post/5aa299c1518825557b4c5806
    // 寻找链表的中间元素
    ListNodePosi(T) MiddleNode()
    {
        ListNodePosi(T) slow = first();
        if (slow->succ_ == trailer_)
        {
            return slow;
        }
        ListNodePosi(T) fast = first()->succ_->succ_;
        while (fast != trailer_)
        {
            slow = slow->succ_;
            fast = (fast->succ_ != trailer_) ? fast->succ_->succ_ : fast->succ_;
        }
        return slow;
    }

    // 将链表尾指针连接到链表第from个节点
    void MakeLoop(int from)
    {
        assert(from <= size());
        ListNodePosi(T) pCur = first();
        for (int i = 1; i < from; ++i)
        {
            pCur = pCur->succ_;
        }    
        last()->succ_ = pCur;
    }
    // 判断链表是否存在环
    bool IsLoop()
    {
        ListNodePosi(T) slow = first();
        // 空表
        if (slow == trailer_) return false;
        // 单个节点成环
        if (slow->succ_ == slow) return true;

        ListNodePosi(T) fast = trailer_;
        if (first()->succ_ != trailer_)
            fast = first()->succ_->succ_;
        while (fast != trailer_ && slow != fast)
        {
            slow = slow->succ_;
            fast = (fast->succ_ != trailer_) ? fast->succ_->succ_ : fast->succ_;
        }
        // 快指针到达尾哨兵
        if (fast == trailer_) return false;
        // 快慢指针相遇 成环
        return true;
    }

    // 单链表求倒数第N个节点
    ListNodePosi(T) LastN(int n)
    {
        ListNodePosi(T) slow = header_;
        ListNodePosi(T) fast = header_;
        while (n-- > 0 && fast != trailer_)
        {
            fast = fast->succ_;
        }

        while (fast != trailer_)
        {
            slow = slow->succ_;
            fast = fast->succ_;
        }
        return slow;
    }
    // 删除链表倒数第N个节点
    bool RemoveLastN(int n)
    {
        ListNodePosi(T) slow = header_;
        ListNodePosi(T) fast = header_;
        while (n-- > 0 && fast != trailer_)
        {
            fast = fast->succ_;
        }

        while (fast != trailer_)
        {
            slow = slow->succ_;
            fast = fast->succ_;
        }

        if (slow == header_) return false;

        ListNodePosi(T) pfront = slow->pred_;
        ListNodePosi(T) prear = slow->succ_;
        pfront->succ_ = prear;
        prear->pred_ = pfront;
        delete slow;
        --size_;
        return true;
    }
    // 链表旋转
    bool Rotate(int n)
    {
        int move = n % size_;
        if (move < 1) return false;
        ListNodePosi(T) slow = first();
        ListNodePosi(T) fast = first();
        while (--move > 0 && fast->succ_ != trailer_) fast = fast->succ_;
        
        if (fast->succ_ == trailer_) return false;
        ListNodePosi(T) front = fast->succ_;
        ListNodePosi(T) last = front;
        while (last != trailer_ && last->succ_ != trailer_) last = last->succ_;

        header_->succ_ = front; front->pred_ = header_;
        last->succ_ = slow; slow->pred_ = last; 
        fast->succ_ = trailer_; trailer_->pred_ = fast;
        return true;
    }

}; // List

}; // namespace MYSTL

#endif