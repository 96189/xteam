#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstddef>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

namespace MYSTL
{

typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template <typename T>
class Vector
{
protected:
    Rank _size;
    int _capacity;
    T* _elem;
protected:
    // 复制数组A区间[lo,hi]
    void copyFrom(const T* A, Rank lo, Rank hi)
    {
        Rank count = hi - lo;
        _capacity = count << 1;
        _elem = new T[_capacity];
        assert(_elem);
        memmove(_elem, A + lo, count * sizeof(T));
        _size = count;
    }
    // 空间不足时扩容
    void expand()
    {
        if (_size == _capacity)
        {
            T *newAddr = new T[_capacity <<= 1];
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
            T *newAddr = new T[_capacity >>= 1];
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
    // // 优化版扫描交换
    int bubble(Rank lo, Rank hi)
    {
        Rank lastSwap = lo;
        Rank idx = lo + 1;
        while (idx < hi)
        {
            if (_elem[idx - 1] > _elem[idx])
            {
                lastSwap = idx;
                swap(_elem[idx - 1], _elem[idx]);
            }
            ++idx;
        }
        return lastSwap;
    }
    // 冒泡排序
    void bubbleSort(Rank lo, Rank hi)
    {
        // 若前半部有序 则中途结束循环
        // 若后缀中已经有序 则快速跳过后缀中有序的部分 避免低效率的挨个比较
        while (lo < (hi = bubble(lo, hi)))
            ;
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
    // 插入排序
    void insertionSort(Rank lo, Rank hi)
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
    void selectionSort(Rank lo, Rank hi)
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
                swap(_elem[maxIdx], _elem[last]);
            }
        }
    }
    // 归并算法
    void merge(Rank lo, Rank mi, Rank hi)
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
    void mergeSort(Rank lo, Rank hi)
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
    // 轴点构造算法 LUG算法(lower, unorder, greater)
    Rank partitionLUG(Rank lo, Rank hi)
    {
        swap(_elem[lo], _elem[lo + rand() % (hi - lo)]);
        T pivotVal = _elem[lo];
        // U _elem[lo, hi-1]
        Rank start = lo;
        Rank end = hi - 1;
        while (start < end)
        {
            // G
            while (start < end && pivotVal <= _elem[end]) --end;
            // G区间发现严格的小于pivotVal的值
            _elem[start] = _elem[end];

            // L
            while (start < end && _elem[start] <= pivotVal) ++start;
            // L区间发现严格的大于pivotVal的值
            _elem[end] = _elem[start];
        } 
        _elem[start] = pivotVal;
        return start;
    }
    // 轴点构造算法 LGU算法
    Rank partitionLGU(Rank lo, Rank hi)
    {
        return 0;
    }
    // 快速排序算法
    void quickSort(Rank lo, Rank hi)
    {
        if (hi - lo <= 1)
        {
            return;
        }
        int pivot = partitionLUG(lo, hi);
        quickSort(lo, pivot);
        quickSort(pivot + 1, hi);
    }
    // 当前idx所在的节点 与两个孩子节点中 三者最大值的秩
    // 秩不能 >= n
    // 无左右孩子的节点 最终返回秩为当前节点秩
    int Maxp2s(T *start, int idx, int n)
    {
        int lcIdx = 2 * idx + 1;
        int rcIdx = 2 * idx + 2;
        int cmpidx = lcIdx;
        // 左孩子不存在(右孩子必不存在))
        if (lcIdx >= n) return idx;
        // 存在右孩子
        if (rcIdx < n)
        {
            cmpidx = start[lcIdx] > start[rcIdx] ? lcIdx : rcIdx;
        }
        // 只存在左孩子 或者已经决出左右孩子的最大者秩
        return start[idx] > start[cmpidx] ? idx : cmpidx;
    }
    void DownFilter(T *start, int idx, int n)
    {
        int maxIdx = idx;
        while (idx != (maxIdx = Maxp2s(start, idx, n)))
        {
            swap(start[idx], start[maxIdx]);
            idx = maxIdx;
        }
    }
    void MakeMaxHeap(T *start, int n)
    {
        for (int idx = n / 2; idx >= 0; --idx)
        {
            DownFilter(start, idx, n);
        }
    }
    // 堆排序算法
    void heapSort(Rank lo, Rank hi)
    {
        MakeMaxHeap(_elem + lo, hi - lo);
        while (--hi > lo)
        {
            swap(_elem[0], _elem[hi]);
            DownFilter(_elem + lo, 0, hi);
        }
    }
public:
    // 构造函数
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = T())
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    Vector(const T* A, Rank lo, Rank hi)
    {
        copyFrom(A, lo, hi);
    }
    Vector(const T* A, Rank n)
    {
        copyFrom(A, 0, n);
    }
    Vector(const Vector<T>& V, Rank lo, Rank hi)
    {
        copyFrom(V._elem, lo, hi);
    }
    // 拷贝构造
    Vector(const Vector<T>& V)
    {
        copyFrom(V._elem, 0, V._size);
    }
    // 析构函数
    ~Vector()
    {
        if (_elem)
        {
            delete[] _elem;
        }
        _elem = NULL;
        _size = _capacity = 0;
    }
    
// 只读访问接口
    Rank size() const 
    { 
        return _size; 
    }
    bool empty() const 
    { 
        return !_size; 
    }
    // 判断向量是否已排序
    int disordered() const
    {
        int counter = 0;
        for (Rank idx = 1; idx < _size; ++idx)
        {
            counter += _elem[idx - 1] > _elem[idx] ? 1 : 0;
        }
        return counter;
    }
    // 无需向量整体查找
    Rank find(const T& e) const 
    { 
        return find(e, 0, _size); 
    }
    // 无序向量区间查找
    // 若存在 则返回第一次出现的位置
    // 若不存在 返回-1
    Rank find(const T& e, Rank lo, Rank hi) const
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
    // 有序向量整体查找
    // 无论存在与否都返回e元素实际应该插入的位置(即第一个大于e的位置)
    Rank search(const T& e) const 
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    // 有序向量区间查找
    // 无论存在与否都返回e元素实际应该插入的位置(即第一个大于e的位置)
    Rank search(const T& e, Rank lo, Rank hi) const
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

    // 可写访问接口
    T & operator[](Rank r) const
    {
        return *(static_cast<T*>(_elem + r));
    }
    Vector<T>& operator=(const Vector<T>& V)
    {
        if (this == &V)
            return;
        copyFrom(V._elem, 0, V._size);
        delete[] V._elem;
        V._elem = NULL;
        return *this;
    }
    void clear()
    {
        remove(0, _size);
    }
    // 删除一段区间
    int remove(Rank lo, Rank hi)
    {
        if (lo == hi)
            return 0;
        while (hi < _size)
        {
            _elem[lo++] = _elem[hi++];
        }
        _size -= (hi - lo);
        return (hi - lo);
    }
    // 删除某一位置的值
    T remove(Rank r)
    {
        T val = _elem[r];
        remove(r, r + 1);
        return val;
    }

    // 插入元素
    Rank insert(Rank r, const T &e)
    {
        expand();
        // 腾出r位置的空间
        // memmove解决内存重叠问题
        memmove(_elem + r + 1, _elem + r, (_size - r) * sizeof(T));
        _elem[r] = e;
        ++_size;
        return 0;
    }
    // 默认作为末尾元素插入
    Rank insert(const T& e)
    {
        return insert(_size, e);
    }
    // 区间排序
    void sort(Rank lo, Rank hi)
    {
        int c = rand() % 1;
        switch (c)
        {
        default:
            // insertionSort(lo, hi);
            // heapSort(lo, hi);
            quickSort(lo, hi);
            // mergeSort(lo, hi);
            // selectionSort(lo, hi);
            // bubbleSort(lo, hi);
            break;
        }
    }
    // 整体排序
    void sort()
    {
        sort(0, _size);
    }
    // 区间置乱
    void unsort(Rank lo, Rank hi)
    {
        T *V = _elem + lo;
        for (Rank i = hi - lo; i > 0; --i)
        {
            swap(V[i - 1], V[rand() % i]);
        }
    }
    // 整体置乱
    void unsort()
    {
        unsort(0, _size);
    }
    // 无序去重
    int deduplicate()
    {
        Rank idx = 1;
        Rank pos = 0;
        Rank oldSize = _size;
        while (idx < _size)
        {
            (pos = find(_elem[idx], 0, idx - 1)) > -1 ? remove(pos) : idx++;
        }
        return oldSize - _size;
    }
    // 有序去重
    int uniquify()
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
    // 逆置
    void reverse()
    {
        for (int s = 0, e = _size - 1; e > s; ++s, --e)
        {
            swap(_elem[s], _elem[e]);
        }
    }
// 遍历
    void traverse(void (*visit)(T &val))
    {
        for (Rank i = 0; i < _size; ++i)
        {
            visit(_elem[i]);
        }
    }
    template<typename VST>
    void traverse(VST& funcobj);
};

};

#endif