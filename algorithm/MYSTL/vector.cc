#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

namespace MYSTL
{
    
// 复制数组A区间[lo,hi]
template <typename T>
void Vector<T>::copyFrom(const T *A, Rank lo, Rank hi)
{
    Rank count = hi - lo;
    _capacity = count << 1;
    _elem = new T[_capacity];
    assert(_elem);
    memmove(_elem, A + lo, count * sizeof(T));
    _size = count; 
}
// 装填因子过小时压缩
template <typename T>
void Vector<T>::shrink()
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
// bool Vector<T>::bubble(Rank lo, Rank hi)
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
// bool Vector<T>::bubbleSort(Rank lo, Rank hi)
// {
//     // 若前半部已经有序 则中途结束循环
//     return while(!bubble(lo, hi--));
// }
// 优化版扫描交换
template <typename T>
int Vector<T>::bubble(Rank lo, Rank hi)
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
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
    // 若前半部有序 则中途结束循环
    // 若后缀中已经有序 则快速跳过后缀中有序的部分 避免低效率的挨个比较
    while (lo < (hi = bubble(lo, hi)));
}
// 选取最大元素
template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi)
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
// 插入排序
template <typename T>
void Vector<T>::insertionSort(Rank lo, Rank hi)
{
    // for (Rank uIdx = lo + 1; uIdx < hi; ++uIdx)
    // {
    //     swap(_elem[search(_elem[uIdx], lo, uIdx) + 1], _elem[uIdx]);
    // }

    // 可优化
    for (Rank uIdx = lo + 1; uIdx < hi; ++uIdx)
    {
        for (Rank oIdx = uIdx; oIdx > lo; --oIdx)
        {
            if (_elem[oIdx - 1] > _elem[oIdx])
            {
                swap(_elem[oIdx - 1], _elem[oIdx]);
            }
            else 
            {
                break;
            }
        }
    }
}
// 选择排序
template <typename T>
void Vector<T>::selectionSort(Rank lo, Rank hi)
{
    Rank maxIdx;
    for (Rank last = hi - 1; last >= lo; --last)
    {
        maxIdx = lo;
        for (Rank idx = lo; idx <= last; ++idx)
        {
            // 算法稳定性保证 取得靠右位置的最大值
            if (_elem[idx] >= _elem[maxIdx])
            {
                maxIdx = idx;
            }
        }
        // 算法稳定性保证 最大的数已经在当前轮靠后的位置不再交换
        if (maxIdx != last)
        {
            swap(_elem[maxIdx],_elem[last]);
        }
    }
}
// 归并算法
template <typename T>
void Vector<T>::merge(Rank lo, Rank mi, Rank hi)
{
    int lenB = mi - lo;
    int lenC = hi - mi;
    T *B = new T[lenB];
    assert(B);
    T *C = new T[lenC];
    assert(C);
    memmove(B, _elem + lo, sizeof(T) * (lenB));
    // 
    memmove(C, _elem + mi, sizeof(T) * (lenC));
   
    int b = 0;
    int c = 0;
    while (b < lenB && c < lenC)
    {
        _elem[lo++] = B[b] < C[c] ? B[b++] : C[c++];
    }
    // B
    while (b < lenB)
    {
        _elem[lo++] = B[b++];
    }
    // C 
    while (c < lenC)
    {
        _elem[lo++] = C[c++];
    }
    delete[] B;
    delete[] C;
}
// 归并排序
template <typename T>
void Vector<T>::mergeSort(Rank lo, Rank hi)
{
    if (lo + 1 >= hi)
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
Rank Vector<T>::partition(Rank lo, Rank hi)
{
    return 0;
}
// 快速排序算法
template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{

}
// 堆排序算法
#define parent(n) (n / 2)
#define leftchild(n) (2 * n + 1)
#define rightChild(n) (2 * n + 2)
template <typename T>
void Vector<T>::heapSort(Rank lo, Rank hi)
{
    
}

// 判断向量是否已排序(升序)
// 0 已排序 
// > 0 未排序(逆序对的个数)
template <typename T>
int Vector<T>::disordered() const
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
Rank Vector<T>::find(const T &e, Rank lo, Rank hi) const
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
// 查找目标元素e 返回不大于e且秩最大的元素的秩
template <typename T>
Rank Vector<T>::search(const T &e, Rank lo, Rank hi) const
{
    Rank mi = 0;
    while (lo < hi)
    {
        mi = (lo + hi) >> 1;
        // 区间划分两半
        // e划分到左半段
        // 右半段值大于e
        _elem[mi] <= e ? lo = mi + 1 : hi = mi; 
    }
    return --lo;
}

// // 可写访问接口
template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T> &V)
{
    if (this == &V) return;
    copyFrom(V._elem, 0, V._size);
    delete[] V._elem;
    V._elem = NULL;
    return *this;
}

// 区间排序
template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{
    int c = rand() % 1;
    switch(c)
    {
        default:
            insertionSort(lo, hi);
            // mergeSort(lo, hi);
            // selectionSort(lo, hi);
            // bubbleSort(lo, hi);
            break;
    }
}

// 区间置乱
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
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
int Vector<T>::deduplicate()
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
int Vector<T>::uniquify()
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

// template <typename VST>
// void Vector<T>::traverse(VST &function)
// {

// }

};


// [start, end)
template<typename T>
void checkVector(MYSTL::Vector<T>& v, const T* arr, int start, int end)
{
    for (int i = start; i < end; ++i)
    {
        assert(v[i-start] == arr[i]);
    }
}
template<typename T>
void checkVector(MYSTL::Vector<T>& v, const MYSTL::Vector<T>& vv, int start, int end)
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
    MYSTL::Vector<int> v;
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
    MYSTL::Vector<int> vv(arr, start, end);
    checkVector(vv, arr, start, end);

    start = 1;
    end = 4;
    MYSTL::Vector<int> vvv(vv, start, end);
    checkVector(vvv, vv, start, end);

    MYSTL::Vector<int> vvvv(vvv);
    checkVector(vvvv, vvv, 0, vvv.size());

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
