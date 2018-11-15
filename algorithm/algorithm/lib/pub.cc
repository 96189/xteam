
//
#include "pub.h"

// O(k)
int MaxIdx(int arr[], int k)
{
    int maxIdx = 0;
    for (int i = 1; i < k; ++i)
    {
        if (arr[i] > arr[maxIdx])
        {
            maxIdx = i;
        }
    }
    return maxIdx;
}

void PrintArray(int arr[], int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf("%2d ", arr[i]);
    }
    printf("\n");
}

// --------------------------------------------------------------------- //
// 父子三者中最大的值下标
// array[] 数组内存起始地址
// idx 当前父节点 在数组中的下标
// size 数组最大的下标值
static int MaxThreeVal(int array[], int idx, int size)
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
// array[] 堆结构的底层存储
// size 堆结构底层存储的长度限制
// idx 当前需要向下筛选的堆节点下标
static void FilterDown(int array[], int size, int idx)
{
    int j = idx;
    while (idx < size && (j = MaxThreeVal(array, idx, size)) != idx)
    {
        Swap(array[j], array[idx]);
        idx = j;
    }
}

// 最大堆构造
// array[] 堆结构底层存储
// 堆结构长度限制
void InitMaxHeap(int array[], const int size)
{
    for (int i = size / 2 - 1; i >= 0; --i)
    {
        FilterDown(array, size, i);
    }
}

// 取最大堆堆顶元素
int MaxHeapTop(int array[])
{
    return array[0];
}

// 向最大堆堆顶添加元素
// array[] 堆结构底层存储
// 
void MaxHeapPush(int array[], int size, int i)
{
    // 交换堆顶元素和待添加元素的位置
    Swap(array[0], array[i]);
    // 将待添加元素 向下筛选
    FilterDown(array, size, 0);
}