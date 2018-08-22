#include "pq_complHeap.h"
#include <stdio.h>

using namespace MYSTL;

void PrintInt(int& val)
{
    printf("%d ", val);
}

int main(int argc, char *argv[])
{
    int arr[] = { 3, 7, 9, 11, 2, 6, 5, 8, 13, 4 };
    int len = sizeof(arr) / sizeof(arr[0]);
    PQ_ComplHeap<int> pq_heap(arr, len);
    pq_heap.showData(PrintInt);
    return 0;
}