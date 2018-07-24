#include "vector.h"

using namespace MYSTL;

// 复制数组A区间[lo,hi]
template <typename T>
void copyFrom(const T *A, Rank lo, Rank hi)
{

}
// 空间不足时扩容
void expand()
{

}
// 装填因子过小时压缩
void shrink()
{

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
    return 0;
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

// 判断向量是否已排序
int disordered() 
{
    return 0;
}
// 无需向量区间查找
template <typename T>
Rank find(const T &e, Rank lo, Rank hi) 
{
    return 0;
}
// 有序向量区间查找
template <typename T>
Rank search(const T &e, Rank lo, Rank hi)
{
    return 0;
}

// 可写访问接口
// template <typename T>
// T &operator[](Rank r) const
// {
//     return 0;
// }
// template <typename T>
// vector<T> &operator=(const vector<T> &V)
// {
// }
// 删除某一位置的值
// template <typename T>
// T remove(Rank r)
// {

// }
// 删除一段区间
int remove(Rank lo, Rank hi)
{
    return 0;
}
// 插入元素
template <typename T>
Rank insert(Rank r, const T &e)
{
    return 0;
}

// 区间排序
void sort(Rank lo, Rank hi)
{

}

// 区间置乱
void unsort(Rank lo, Rank hi)
{

}

// 无序去重
int deduplicate()
{
    return 0;
}
// 有序去重
int uniquify()
{
    return 0;
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