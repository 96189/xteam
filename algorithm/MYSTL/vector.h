#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstddef>
#include <string.h>
#include <assert.h>
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
    void shrink();
    // 扫描交换
    int bubble(Rank lo, Rank hi);
    // 冒泡排序
    void bubbleSort(Rank lo, Rank hi);
    // 选取最大元素
    Rank max(Rank lo, Rank hi);
    // 插入排序
    void insertionSort(Rank lo, Rank hi);
    // 选择排序
    void selectionSort(Rank lo, Rank hi);
    // 归并算法
    void merge(Rank lo, Rank mi, Rank hi);
    // 归并排序
    void mergeSort(Rank lo, Rank hi);
    // 轴点构造算法
    Rank partition(Rank lo, Rank hi);
    // 快速排序算法
    void quickSort(Rank lo, Rank hi);
    // 堆排序算法
    void heapSort(Rank lo, Rank hi);
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
    int disordered() const;
    // 无需向量整体查找
    Rank find(const T& e) const 
    { 
        return find(e, 0, _size); 
    }
    // 无序向量区间查找
    // 若存在 则返回第一次出现的位置
    // 若不存在 返回-1
    Rank find(const T& e, Rank lo, Rank hi) const;
    // 有序向量整体查找
    // 无论存在与否都返回e元素实际应该插入的位置(即第一个大于e的位置)
    Rank search(const T& e) const 
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    // 有序向量区间查找
    // 无论存在与否都返回e元素实际应该插入的位置(即第一个大于e的位置)
    Rank search(const T& e, Rank lo, Rank hi) const;

// 可写访问接口
    T & operator[](Rank r) const
    {
        return *(static_cast<T*>(_elem + r));
    }
    Vector<T>& operator=(const Vector<T>& V);
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
    void sort(Rank lo, Rank hi);
    // 整体排序
    void sort()
    {
        sort(0, _size);
    }
    // 区间置乱
    void unsort(Rank lo, Rank hi);
    // 整体置乱
    void unsort()
    {
        unsort(0, _size);
    }
    // 无序去重
    int deduplicate();
    // 有序去重
    int uniquify();
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