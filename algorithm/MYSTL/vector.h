#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <cstddef>
namespace MYSTL 
{

typedef int Rank;
#define DEFAULT_CAPACITY 3

template <typename T>
class vector
{
private:
    Rank _size;
    int _capacity;
    T* _elem;
protected:
    // 复制数组A区间[lo,hi]
    void copyFrom(const T* A, Rank lo, Rank hi);
    // 空间不足时扩容
    void expand();
    // 装填因子过小时压缩
    void shrink();
    // 扫描交换
    bool bubble(Rank lo, Rank hi);
    // 冒泡排序
    bool bubbleSort(Rank lo, Rank hi);
    // 选取最大元素
    Rank max(Rank lo, Rank hi);
    // 选择排序
    bool selectionSort(Rank lo, Rank hi);
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
    vector(int c = DEFAULT_CAPACITY, int s = 0, T v = T())
    {
        _elem = new T[_capacity = c];
        for (_size = 0; _size < s; _elem[_size++] = v);
    }
    vector(const T* A, Rank lo, Rank hi)
    {
        copyFrom(A, lo, hi);
    }
    vector(const T* A, Rank n)
    {
        copyFrom(A, 0, n);
    }
    vector(const vector<T>& V, Rank lo, Rank hi)
    {
        copyFrom(V._elem, lo, hi);
    }
    // 拷贝构造
    vector(const vector<T>& V)
    {
        copyFrom(V._elem, 0, V._size);
    }
    // 析构函数
    ~vector()
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
    // 无需向量区间查找
    Rank find(const T& e, Rank lo, Rank hi) const;
    // 有序向量整体查找
    Rank search(const T& e) const 
    {
        return (0 >= _size) ? -1 : search(e, 0, _size);
    }
    // 有序向量区间查找
    Rank search(const T& e, Rank lo, Rank hi);

// 可写访问接口
    T& operator[](Rank r) const;
    vector<T>& operator=(const vector<T>& V);
    // 删除某一位置的值
    T remove(Rank r);
    // 删除一段区间
    int remove(Rank lo, Rank hi);
    // 插入元素
    Rank insert(Rank r, const T& e);
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

// 遍历
    void traverse(void (*pfun)(T& val));
    template<typename VST>
    void traverse(VST& function);
};

};

#endif