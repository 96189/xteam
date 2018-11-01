
#include "minK.h"

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

void TestMinK()
{
    const int len = 10;
    const int k = 5;
    int arr[len] = {0};
    for (int i = 0; i < len; ++i)
    {
        arr[i] = rand() % MOD;
    }
    PrintArray(arr, len);
    // MinKV1(arr, len, k);
    MinKV2(arr, len, k);
    PrintArray(arr, k);
}