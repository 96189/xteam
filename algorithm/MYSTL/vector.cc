#include "vector.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

namespace MYSTL
{

template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// 复制数组A区间[lo,hi]
template <typename T>
void vector<T>::copyFrom(const T *A, Rank lo, Rank hi)
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
void vector<T>::expand()
{
    if (_size == _capacity)
    {
        T* newAddr = new T[_capacity<<=1];
        assert(newAddr);
        memmove(newAddr, _elem, _size * sizeof(T));
        delete[] _elem;
        _elem = newAddr;
    }
}
// 装填因子过小时压缩
template <typename T>
void vector<T>::shrink()
{
    if (_size * 2 < _capacity)
    {
        T* newAddr = new T[_capacity>>=1];
        assert(newAddr);
        memmove(newAddr, _elem, _size * sizeof(T));
        delete[] _elem;
        _elem = newAddr;        
    }
}
// // 扫描交换
// bool vector<T>::bubble(Rank lo, Rank hi)
// {
//     bool sorted = true;
//     Rank idx = lo + 1;
//     while (idx < hi)
//     {
//         if (_elem[idx-1] > _elem[idx])
//         {
//             sorted = false;
//             swap(_elem[idx-1], _elem[idx]);
//         }
//         ++idx;
//     }
//     return sorted;
// }
// // 冒泡排序
// bool vector<T>::bubbleSort(Rank lo, Rank hi)
// {
//     // 若前半部已经有序 则中途结束循环
//     return while(!bubble(lo, hi--));
// }
// 优化版扫描交换
template <typename T>
int vector<T>::bubble(Rank lo, Rank hi)
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
// 优化版冒泡排序
template <typename T>
void vector<T>::bubbleSort(Rank lo, Rank hi)
{
    // 若前半部有序 则中途结束循环
    // 若后缀中已经有序 则快速跳过后缀中有序的部分 避免低效率的挨个比较
    while (lo < (hi = bubble(lo, hi)));
}
// 选取最大元素
template <typename T>
Rank vector<T>::max(Rank lo, Rank hi)
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
template <typename T>
void vector<T>::selectionSort(Rank lo, Rank hi)
{
    
}
// 归并算法
template <typename T>
void vector<T>::merge(Rank lo, Rank mi, Rank hi)
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
template <typename T>
void vector<T>::mergeSort(Rank lo, Rank hi)
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
template <typename T>
Rank vector<T>::partition(Rank lo, Rank hi)
{
    return 0;
}
// 快速排序算法
template <typename T>
void vector<T>::quickSort(Rank lo, Rank hi)
{

}
// 堆排序算法
template <typename T>
void vector<T>::heapSort(Rank lo, Rank hi)
{

}

// 判断向量是否已排序(升序)
// 0 已排序 
// > 0 未排序(逆序对的个数)
template <typename T>
int vector<T>::disordered() const
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
Rank vector<T>::find(const T &e, Rank lo, Rank hi) const
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
// template <typename T>
// Rank vector<T>::BinSearchV1(const T &e, Rank lo, Rank hi)
// {
//     Rank pos = -1;
//     Rank mi = 0;
//     while (lo <= hi)
//     {
//         // 二分查找 选取中间位置
//         mi = (lo + hi) >> 1;
//         if (e < _elem[mi])
//         {
//             hi = mi - 1;
//         }
//         else if (_elem[mi] < e)
//         {
//             lo = mi + 1;
//         }
//         else 
//         {
//             pos = mi;
//             break;
//         }
//     }
//     return pos;
// }
// template <typename T>
// Rank vector<T>::BinSearchV2(const T &e, Rank lo, Rank hi)
// {
//     return 0;
// }
// template <typename T>
// Rank vector<T>::BinSearchV3(const T &e, Rank lo, Rank hi)
// {
//     return 0;
// }
// template <typename T>
// Rank vector<T>::FiboSearch(const T &e, Rank lo, Rank hi)
// {
//     // fibonacii查找 选取黄金分割点
//     return 0;
// }
// 有序向量区间查找
// 查找目标元素e 返回不大于e且秩最大的元素的秩
template <typename T>
Rank vector<T>::search(const T &e, Rank lo, Rank hi) const
{
    Rank m = lo;
    while (lo < hi - 1)
    {
        m = (lo + hi) >> 1;
        if (e < _elem[m])
        {
            hi = m - 1;
        }
        else 
        {
            lo = m;
        }
    }
    return m;
}

// 可写访问接口
template <typename T>
T &vector<T>::operator[](Rank r) const
{
    return *(static_cast<T*>(_elem + r));
}
template <class T>
vector<T>& vector<T>::operator=(const vector<T> &V)
{
    if (this == &V) return;
    copyFrom(V._elem, 0, V._size);
    delete[] V._elem;
    V._elem = NULL;
    return *this;
}
// 删除某一位置的值
template <typename T>
T vector<T>::remove(Rank r)
{
    T val = _elem[r];
    remove(r, r + 1);
    return val;
}
// 删除一段区间
template <typename T>
int vector<T>::remove(Rank lo, Rank hi)
{
    if (lo == hi) return 0;
    while (hi < _size)
    {
        _elem[lo++] = _elem[hi++];
    }
    _size -= (hi - lo);
    return (hi - lo);
}
// 插入元素
template <typename T>
Rank vector<T>::insert(Rank r, const T &e)
{
    expand();
    // 腾出r位置的空间
    // memmove解决内存重叠问题
    memmove(_elem + r + 1, _elem + r, (_size - r) * sizeof(T));
    _elem[r] = e;
    ++_size;
    return 0;
}

// 区间排序
template <typename T>
void vector<T>::sort(Rank lo, Rank hi)
{
    int c = rand() % 1;
    switch(c)
    {
        default:
            bubbleSort(lo, hi);
            break;
    }
}

// 区间置乱
template <typename T>
void vector<T>::unsort(Rank lo, Rank hi)
{
    T* V = _elem + lo;
    for (Rank i = hi - lo; i > 0; --i)
    {
        swap(V[i - 1], V[rand() % i]);
    }
}

// 无序去重
// O(n^2)优化
template <typename T>
int vector<T>::deduplicate()
{
    Rank idx = 1;
    Rank pos = 0;
    Rank oldSize = _size;
    while (idx < _size)
    {
        (pos = find(_elem[idx], 0, idx-1)) > -1 ? remove(pos) : idx++;
    }
    return oldSize - _size;
}
// 有序去重
template <typename T>
int vector<T>::uniquify()
{
    Rank i = 0;
    for (int j = 1; j < _size; ++j)
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
void vector<T>::traverse(void (*visit)(T &val))
{
    for (Rank i = 0; i < _size; ++i)
    {
        visit(_elem[i]);
    }
}
// template <typename VST>
// void vector<T>::traverse(VST &function)
// {

// }

};

// [start, end)
template<typename T>
void checkvector(MYSTL::vector<T>& v, const T* arr, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        assert(v[i-start] == arr[i]);
    }
}
template<typename T>
void checkvector(MYSTL::vector<T>& v, const MYSTL::vector<T>& vv, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        assert(v[i-start] == vv[i]);
    }
}
void PrintInt(int& val)
{
    printf("%d ", val);
}
int main(int argc, char* argv[])
{
    const int n = 11;
    MYSTL::vector<int> v;
    assert(v.size() == 0 && v.empty());
    // 包含0个逆序对
    for (int i = 0; i < n; ++i)
    {
        v.insert(i,i);
        assert(v[i] == i);
    }

    int arr[n] = { 3, 22, 55, 11, 1, 66, 71, 89, 9, 10, 10 };
    int start = 3;
    int end = 9;
    // 11, 1, 66, 71, 89, 9 => 包含2个逆序对
    MYSTL::vector<int> vv(arr, start, end);
    checkvector(vv, arr, start, end);

    start = 1;
    end = 4;
    MYSTL::vector<int> vvv(vv, start, end);
    checkvector(vvv, vv, start, end);

    MYSTL::vector<int> vvvv(vvv);
    checkvector(vvvv, vvv, 0, vvv.size());

    int pair = v.disordered();
    assert(pair == 0);
    pair = vv.disordered();
    assert(pair == 2);

    // 11, 1, 66, 71, 89, 9
    int idx = vv.find(3);
    assert(idx == -1);
    idx = vv.find(66);
    assert(idx == 2);

    int count = vv.remove(2, 4);
    assert(count == (4-2) && vv.size() == 4);

    // 11, 1, 89, 9
    int val = vv.remove(2);
    assert(vv.size() == 3 && val == 89);

    // 11, 1, 9
    vv.insert(11);
    vv.insert(1);
    vv.insert(9);
    assert(vv.size() == 6);

    count = vv.deduplicate();
    assert(count == 3 && vv[0] == 11 && vv[1] == 1 && vv[2] == 9);

    vv.insert(11);
    vv.sort();
    vv.traverse(PrintInt);
    // 1 9 11 11
    assert(vv.search(11) == 3);
    assert(vv.search(8) == 0);
    printf("\n");
    vv.unsort();
    vv.traverse(PrintInt);
    return 0;
}