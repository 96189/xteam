#include "list.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

using namespace MYSTL;

// 创建列表时的初始化
void init();
// 清除所有节点
int clear();
// 复制列表中自位置P起的n项
void copyNodes(ListNodePosi(T) p, int n);
// 有序列表区间归并
void merge(ListNodePosi(T) &, int, List<T> &, ListNodePosi(T), int);
// 对从p开始的连续n个节点归并排序
void mergeSort(ListNodePosi(T) &, int);
// 对从p开始的连续n个节点选择排序
void selectionSort(ListNodePosi(T), int);
// 对从p开始的连续n个节点插入排序
void insertionSort(ListNodePosi(T), int);
