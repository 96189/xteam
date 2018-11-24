
//

#include <stdlib.h>
#include <stdio.h>
#include <cctype>
#include <cstring>
#include <iostream>
#include <vector>

// 数值交换
template <typename T>
void Swap(T& valA, T& valB)
{
    T tmp = valA;
    valA = valB;
    valB = tmp;
}

// 区间逆置
// seq[start, end]
template <typename T>
void reverse(T& seq, int start,int end)
{
    for (int i = start, j = end; i < j; ++i, --j)
    {
        Swap(seq[i], seq[j]);       
    }
}

// 向量输出 
template <typename T>
void PrintVector(std::vector<T>& vec)
{   
    for (unsigned int i = 0; i < vec.size(); ++i)
    {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
int MaxValueIdx(std::vector<T>& arr)
{
    int maxIdx = 0;
    for (unsigned int i = 1; i < arr.size(); ++i)
    {
        if (arr[i] > arr[maxIdx])
        {
            maxIdx = i;
        }
    }
    return maxIdx;    
}

// --------------------------------------------------------------------- //
// 父子三者中最大的值下标
// idx 当前父节点 在数组中的下标
// size 数组最大的下标值
template <typename T>
int MaxThreeValIdx(std::vector<T>& array, int idx, int size)
{
    int maxIdx = -1;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // 左右孩子都存在
    if (right < size)
    {
        maxIdx = array[right] > array[left] ? right : left;
    }
    // 至少存在一个左孩子
    else if (left < size)
    {
        maxIdx = array[left] > array[idx] ? left : idx;
    }
    // 不存在任何孩子
    else 
    {
        maxIdx = idx;
    }
    return maxIdx;
}

// 向下筛选
// size 堆结构底层存储的长度限制
// idx 当前需要向下筛选的堆节点下标
template <typename T>
void FilterDown(std::vector<T>& array, int size, int idx)
{
    int j = idx;
    while (idx < size && (j = MaxThreeValIdx(array, idx, size)) != idx)
    {
        Swap(array[j], array[idx]);
        idx = j;
    }
}

// 最大堆构造
// 堆结构长度限制
template <typename T>
void InitMaxHeap(std::vector<T>& array, const int size)
{
    for (int i = size / 2 - 1; i >= 0; --i)
    {
        FilterDown(array, size, i);
    }
}

// 取最大堆堆顶元素
template <typename T>
T MaxHeapTop(std::vector<int>& array)
{
    return array[0];
}

// 向最大堆堆顶添加元素
template <typename T>
void MaxHeapPush(std::vector<int>& array, int size, int i)
{
    // 交换堆顶元素和待添加元素的位置
    Swap(array[0], array[i]);
    // 将待添加元素 向下筛选
    FilterDown(array, size, 0);
}

void Upper(char* str);


