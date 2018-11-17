
#include "array.h"

// 输入n个整数，输出其中最小的k个。

// O(k) + (n-k) * O(k)
void MinKV1(int arr[], int len, int k)
{
    // [0,k)  [k,len)
    int maxIdx = MaxIdx(arr, k);
    for (int i = k; i < len; ++i)
    {
        if (arr[i] < arr[maxIdx])
        {
            Swap(arr[maxIdx], arr[i]);
            // O(k)
            maxIdx = MaxIdx(arr, k);
        }
    }
}

// O(log(k)) + (n-k) * O(log(k))
void MinKV2(int arr[], int len, int k)
{
    InitMaxHeap(arr, k);
    for (int i = k; i < len; ++i)
    {
        if (arr[i] < MaxHeapTop(arr))
        {
            MaxHeapPush(arr, k, i);
        }
    }
}

// array为有序序列 diff必为有序序列
// 有序序列
// 空间复杂度 O(N)
// 时间复杂度 O(N)
bool FindTwoSumN_v1(const int array[], const int len, const int n, int res[])
{
    // 构造差值表
    int *diff = new int[len];   // O(N)
    for (int i = 0; i < len; ++i)
    {
        diff[i] = n - array[i];
    }
    
    // 同时遍历源数据表array[i]和差值表diff[j]
    int i = 0, j = len-1; 
    int count = 0;
    // O(N)
    while (i < len && j >= 0)
    {
        if (array[i] > diff[j]) 
        {
            --j;
        }
        else if (array[i] < diff[j])
        {
            ++i;
        }
        else
        {
            res[count++] = array[i];
            ++i; 
            --j;
        } 
    }
    delete diff;
    return (count == 2);
}

// 有序序列
// 时间复杂度 O(N)
// 空间复杂度 O(1)
bool FindTwoSumN_v2(const int array[], const int len, const int n, int res[])
{
    int i = 0;
    int j = len-1;
    int count = 0;  // O(1)
    // O(N)
    while (i < j)
    {
        if (array[i] + array[j] > n)
        {
            --j;
        }
        else if (array[i] + array[j] < n)
        {
            ++i;
        }
        else
        {
            res[count++] = array[i];
            res[count++] = array[j];
            break;
        }
    }
    return (count == 2);
}



