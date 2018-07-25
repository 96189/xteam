#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

using namespace MYSTL;

// 复制数组A区间[lo,hi]
template <typename T>
void copyFrom(const T *A, Rank lo, Rank hi)
{
    // 
    Rank count = hi - lo;
    _capacity = count << 1;
    _elem = new T[_capacity];
    assert(_elem);
    for (_size = 0; _size < count; ++_size)
    {
        _elem[_size] = A[lo + _size];
    }  
}
// 空间不足时扩容
template <typename T>
void expand()
{
    if (_size == _capacity)
    {
        T* newAddr = new T[_capacity=<<1];
        assert(newAddr);
        memcpy(newAddr, _elem, _size * sizeof(T));
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
        memcpy(newAddr, _elem, _size * sizeof(T));
        delete[] _elem;
        _elem = newAddr;        
    }
}
// 扫描交换
bool bubble(Rank lo, Rank hi)
{
    return true;
}
// 冒泡排序
bool bubbleSort(Rank lo, Rank hi)
{
    return true;
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

}
// 归并排序
void mergeSort(Rank lo, Rank hi)
{

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
// 有序向量区间查找
template <typename T>
Rank search(const T &e, Rank lo, Rank hi)
{
    Rank pos = -1;
    Rank mi = 0;
    while (lo <= hi)
    {
        // 二分查找 选取中间位置
        mi = (lo + hi) >> 1;
        // fibonacii查找 选取黄金分割点
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
    // memcpy解决内存重叠问题
    memcpy(_elem + r, _elem + r + 1, (_size - r) * sizeof(T));
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
        T tmp = _elem[i];
        _elem[i] = _elem[r];
        _elem[r] = tmp;
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