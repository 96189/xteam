
//

#include <stdlib.h>
#include <stdio.h>
#include <cctype>
#include <cstring>

// 交换
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

int MaxIdx(int arr[], int k);

void PrintArray(int arr[], int len);

void Upper(char* str);

// 最大堆构造
void InitMaxHeap(int array[], const int size);
// 取最大堆中的堆顶元素
int MaxHeapTop(int array[]);
// 向最大堆中添加元素
void MaxHeapPush(int array[], int size, int i);
