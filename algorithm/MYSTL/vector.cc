#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

using namespace MYSTL;

// 复制数组A区间[lo,hi]
template <typename T>
void copyFrom(const T *A, Rank lo, Rank hi)
{
    Rank count = hi - lo;
    _capacity = count << 1;
    _elem = new T[_capacity];
    assert(_elem);
    memmove(_elem, A + lo, count * sizeof(T));
    _size = count; 
}
// 空间不足时扩容
template <typename T>
void expand()
{
    if (_size == _capacity)
    {
        T* newAddr = new T[_capacity=<<1];
        assert(newAddr);
        memmove(newAddr, _elem, _size * sizeof(T));
        delete[] _elem;
        _elem = newAddr;
    }
}
// 装填因子过小时压缩
void shrink()
{
    if (_size * 2 < _capacity)
    {
        T* newAddr = new T[_capacity=>>1];
        assert(newAddr);
        memmove(newAddr, _elem, _size * sizeof(T));
        delete[] _elem;
        _elem = newAddr;        
    }
}
// 扫描交换
bool bubbleV1(Rank lo, Rank hi)
{
    bool sorted = true;
    Rank idx = lo + 1;
    while (idx < hi)
    {
        if (_elem[idx-1] > _elem[idx])
        {
            sorted = false;
            swap(_elem[idx-1], _elem[idx]);
        }
        ++idx;
    }
    return sorted;
}
// 冒泡排序
bool bubbleSortV1(Rank lo, Rank hi)
{
    // 若前半部已经有序 则中途结束循环
    return while(!bubbleV1(lo, hi--));
}
bool bubbleV2(Rank lo, Rank hi)
{
    Rank lastSwap = lo;
    Rank idx = lo + 1;
    while (idx < hi)
    {
        if (_elem[idx-1] > _elem[idx])
        {
            lastSwap = idx;
            swap(_elem[idx-1], _elem[idx]);
        }
        ++idx;
    }
    return lastSwap;
}
bool bubbleSortV2(Rank lo, Rank hi)
{
    // 若前半部有序 则中途结束循环
    // 若后缀中已经有序 则快速跳过后缀中有序的部分 避免低效率的挨个比较
    return while(lo < (hi = bubbleV2(lo, hi)));
}
// 选取最大元素
Rank max(Rank lo, Rank hi)
{
    Rank maxIdx = hi - 1;
    for (int idx = maxIdx - 1; idx >= lo; --idx)
    {
        if (_elem[idx] > _elem[maxIdx])
        {
            maxIdx = idx;
        }
    }
    return maxIdx;
}
// 选择排序
bool selectionSort(Rank lo, Rank hi)
{
    return true;
}
// 归并算法
void merge(Rank lo, Rank mi, Rank hi)
{
    T *B = new T[mi - lo];
    T *C = _elem + lo + mi;
    assert(B);
    int b = lo;
    int c = mi;
    while (b < mi && c < hi)
    {
        T maxVal = B[b] > _elem[c] ? B[b++] : _elem[c++];
        _elem[lo++] = maxVal; 
    }
    // B
    while (b < mi)
    {
        _elem[lo++] = B[b++];
    }
    // C -> nothing
    delete[] B;
}
// 归并排序
void mergeSort(Rank lo, Rank hi)
{
    if (hi <= lo)
    {
        return;
    }
    int mi = (lo + hi) >> 1;
    mergeSort(lo, mi);
    mergeSort(mi, hi);
    merge(lo, mi, hi);
}
// 轴点构造算法
Rank partition(Rank lo, Rank hi)
{
    return 0;
}
// 快速排序算法
void quickSort(Rank lo, Rank hi)
{

}
// 堆排序算法
void heapSort(Rank lo, Rank hi)
{

}

// 判断向量是否已排序(升序)
// 0 已排序 
// > 0 未排序(逆序对的个数)
int disordered() 
{
    int counter = 0;
    for (Rank idx = 1; idx < _size; ++idx)
    {
        counter += _elem[idx - 1] > _elem[idx] ? 1 : 0;
    }
    return counter;
}
// 无需向量区间查找
template <typename T>
Rank find(const T &e, Rank lo, Rank hi) 
{
    Rank pos = -1;
    for (int i = lo; i < hi; ++i)
    {
        if (_elem[i] == e)
        {
            pos = i;
            break;
        }
    }
    return pos;
}
template <typename T>
Rank BinSearchV1(const T &e, Rank lo, Rank hi)
{
    Rank pos = -1;
    Rank mi = 0;
    while (lo <= hi)
    {
        // 二分查找 选取中间位置
        mi = (lo + hi) >> 1;
        if (e < _elem[mi])
        {
            hi = mi - 1;
        }
        else if (_elem[mi] < e)
        {
            lo = mi + 1;
        }
        else 
        {
            pos = mi;
            break;
        }
    }
    return pos;
}
template <typename T>
Rank BinSearchV2(const T &e, Rank lo, Rank hi)
{

}
template <typename T>
Rank BinSearchV3(const T &e, Rank lo, Rank hi)
{
    
}
template <typename T>
Rank FiboSearch(const T &e, Rank lo, Rank hi)
{
    // fibonacii查找 选取黄金分割点
}
// 有序向量区间查找
template <typename T>
Rank search(const T &e, Rank lo, Rank hi)
{
    Rank idx = -1;
    Rank r = rand() % 4;
    switch(r)
    {
        case 1:
            idx = BinSearchV1(e, lo, hi);
            break;
        case 2:
            idx = BinSearchV2(e, lo, hi);
            break;
        case 3:
            idx = BinSearchV3(e, lo, hi);
            break;
        default:
            idx = FiboSearch(e, lo, hi);
            break;
    }
    return idx;
}

// 可写访问接口
template <typename T>
T &operator[](Rank r) const
{
    return static_cast<T*>(_elem + r);
}
template <typename T>
vector<T> &operator=(const vector<T> &V)
{
    if (this == &V) return;
    copyFrom(V._elem, 0, V._size);
    delete[] V._elem;
    V._elem = NULL;
    return *this;
}
// 删除某一位置的值
template <typename T>
T remove(Rank r)
{
    T val = _elem[r];
    remove(r, r + 1);
    return val;
}
// 删除一段区间
int remove(Rank lo, Rank hi)
{
    if (lo == hi) return 0;
    while (hi < _size)
    {
        _elem[lo++] = _elem[hi++];
    }
    return (hi - lo);
}
// 插入元素
template <typename T>
Rank insert(Rank r, const T &e)
{
    expand();
    // memmove解决内存重叠问题
    memmove(_elem + r, _elem + r + 1, (_size - r) * sizeof(T));
    _elem[r] = e;
    return 0;
}

// 区间排序
void sort(Rank lo, Rank hi)
{

}

// 区间置乱
void unsort(Rank lo, Rank hi)
{
    for (Rank i = hi - 1; i >= lo; --i)
    {
        Rank r = rand() % i;
        swap(_elem[r],_elem[i]);
    }
}

// 无序去重
int deduplicate()
{
    
    return 0;
}
// 有序去重
int uniquify()
{
    Rank i = 0;
    for (j = 1; j < _size; ++j)
    {
        if (_elem[i] != _elem[j])
        {
            _elem[++i] = _elem[j];
        }
    }
    _size = i;
    return i;
}

// 遍历
template <typename T>
void traverse(void (*pfun)(T &val))
{

}
template <typename VST>
void traverse(VST &function)
{

}