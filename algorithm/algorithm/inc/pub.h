
//

#include <stdlib.h>
#include <stdio.h>

template <typename T>
void Swap(T& valA, T& valB)
{
    T tmp = valA;
    valA = valB;
    valB = tmp;
}

int MaxIdx(int arr[], int k);

void PrintArray(int arr[], int len);

// 最大堆构造
void InitMaxHeap(int array[], const int size);
// 取最大堆中的堆顶元素
int MaxHeapTop(int array[]);
// 向最大堆中添加元素
void MaxHeapPush(int array[], int size, int i);
