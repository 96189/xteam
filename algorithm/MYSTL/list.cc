#include "list.h"

void PrintInt(int& val)
{
    printf("%d ", val);
}
namespace MYSTL
{
// 有序列表区间归并
template <typename T>
void List<T>::merge(ListNodePosi(T) &, int, List<T> &, ListNodePosi(T), int)
{

}
// 对从p开始的连续n个节点归并排序
template <typename T>
void List<T>::mergeSort(ListNodePosi(T) &, int)
{

}
// 对从p开始的连续n个节点选择排序
template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
    int count = n;
    ListNodePosi(T) ofirst = p;
    while (--count)
    {
        ofirst = ofirst->succ;
    }
    for (ListNodePosi(T) ulast = ofirst; /* 初始状态有序空间为0 无序空间占据整个空间 */
         ulast != p; /* 有序空间未填充整个无序空间 */
         ulast = ulast->pred) /* 每次循环有序空间增长 无序空间减小 */
    {
        // 在[ufirst, ulast]区间中找最大值
        ListNodePosi(T) pMax = p;
        for (ListNodePosi(T) ufirst = p ; 
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
template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n)
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

// 重载[]支持寻秩访问
template <typename T>
T &List<T>::operator[](Rank r) const
{
    assert(r < _size);
    ListNodePosi(T) pCur = header->succ;
    while(r-- > 0)
    {
        pCur = pCur->succ;
    }
    return pCur->getData();
}

// 判断位置p是否对外合法
template <typename T>
bool List<T>::valid(ListNodePosi(T) p) const
{
    return (p && p != header && p != trailer);
}
// 判断列表是否已经有序
template <typename T>
int List<T>::disordered() const
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
template <typename T>
ListNodePosi(T) List<T>::find(const T &e) const
{
    return find(e, _size, last());
}
// 无序区间查找
template <typename T>
ListNodePosi(T) List<T>::find(const T &e, int n, ListNodePosi(T) p) const
{
    assert(n > 0 && p);
    ListNodePosi(T) pCur = p->succ; 
    while ((pCur = pCur->pred) != trailer && --n)
    {
        if (pCur->getData() == e) break;
    }
    if (n == 0 && pCur->getData() != e)
    {
        return NULL;
    }
    return pCur;
}
// 有序列表查找
template <typename T>
ListNodePosi(T) List<T>::search(const T &e)
{
    return search(e, _size, last());
}
// 有序区间查找(包含p位置的p的前n个节点)
// 查找第一个不大于e的节点的地址
template <typename T>
ListNodePosi(T) List<T>::search(const T &e, int n, ListNodePosi(T) p) const
{
    assert(n > 0 && p);
    ListNodePosi(T) pCur = p->succ; 
    while ((pCur = pCur->pred) != trailer && --n)
    {
        if (pCur->getData() <= e) break;
    }
    return pCur;
}
// 在p及其n-1个后继中选出最大者
template <typename T>
T List<T>::selectMax(ListNodePosi(T) p, int n)
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
template <typename T>
T List<T>::selectMax()
{
    return selectMax(first(), _size);
}
// 可写访问接口
// 将e当作p的前驱插入
template <typename T>
void List<T>::insertAsPred(ListNodePosi(T) p, const T &e)
{
    p->insertAsPred(e);
    ++_size;
}
// 将e当作p的后继插入
template <typename T>
void List<T>::insertAsSucc(ListNodePosi(T) p, const T &e)
{
    p->insertAsSucc(e);
    ++_size;
}
// 全列表归并
template <typename T>
void List<T>::merge(List<T> &L)
{

}
// 列表区间排序
template <typename T>
void List<T>::sort(ListNodePosi(T) s, int n)
{
    selectionSort(s, n);
    // insertionSort(s, n);
}
// 列表整体排序
template <typename T>
void List<T>::sort()
{
    sort(header->succ, _size);
}
// 无序去重
template <typename T>
int List<T>::deduplicate()
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
template <typename T>
int List<T>::uniquify()
{
    return 0;
}
template <typename T>
void swap(ListNodePosi(T) first, ListNodePosi(T) last)
{
    T val = first->getData();
    first->setData(last->getData());
    last->setData(val);
}
// 前后逆置
template <typename T>
void List<T>::reverse()
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
// 函数对象遍历
template <typename VST>
void traverse(VST &funobj)
{

}
};

using namespace MYSTL;
int main(int argc, char* argv[])
{
    MYSTL::List<int> l;
    assert(l.size() == 0);
    assert(sizeof(l) == 24 || sizeof(l) == 12);
    assert(l.size() == 0 && l.empty());
    // 头插
    l.insertAsFirst(5);
    l.insertAsFirst(3);
    l.insertAsFirst(1);
    l.traverse(PrintInt);
    printf("\n");
    assert(l.first()->getData() == 1 && l.last()->getData() == 5);
    assert(l.size() == 3);
    // 尾插
    l.insertAsLast(9);
    l.insertAsLast(2);
    l.insertAsLast(4);
    l.insertAsLast(6);
    l.traverse(PrintInt);
    printf("\n");
    assert(l.first()->getData() == 1 && l.last()->getData() == 6);
    assert(l.size() == 7);
    
    // 列表查找 指定位置前插入
    ListNodePosi(int) pCur = l.find(5);
    assert(pCur->getData() == 5);
    l.insertAsSucc(pCur, 5);

    // 列表查找 指定位置之后插入
    pCur = l.find(2, 3, l.last());
    assert(pCur->getData() == 2);
    l.insertAsPred(pCur, 2);

    l.traverse(PrintInt);
    printf("\n");

    // 1 3 5 5 9 2 2 4 6
    int count = l.disordered();
    assert(count == 1);
    
    pCur = l.find(9);
    int val = l.remove(pCur);
    // assert(l.valid(pCur) == false);
    l.traverse(PrintInt);
    printf("\n");

    l.reverse();
    l.traverse(PrintInt);
    printf("\n");

    l.reverse();
    l.traverse(PrintInt);
    printf("\n");

    val = l.selectMax();
    assert(val == 6);

    printf("copy construct\n");
    List<int> ll(l);
    List<int> lll(l, 0, l.size());
    List<int> llll(l.first(), l.size());
    ll.traverse(PrintInt);
    printf("\n");
    lll.traverse(PrintInt);
    printf("\n");
    llll.traverse(PrintInt);
    printf("\n");

    // sort
    printf("sort\n");
    l.sort();
    l.traverse(PrintInt);
    printf("\n");

    // 1 3 5 5 2 2 4 6
    count = l.deduplicate();
    assert(count == 2);
    l.traverse(PrintInt);
    printf("\n");
    return 0;
}