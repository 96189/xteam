#include "list.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>

namespace MYSTL
{

// 构造函数 针对header和trailer的构造
template <typename T>
ListNode<T>::ListNode() {}

template <typename T>
ListNode<T>::ListNode(T e, ListNodePosi(T) p, ListNodePosi(T) s)
    : data(e), pred(p), succ(s)
{

}

// 在当前节点之前插入
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(const T &e)
{
    return NULL;
}
// 在当前节点之后插入
template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(const T &e)
{
    return NULL;
}

/////////////////////////////////////////////////////////////////
// 创建列表时的初始化
template <typename T>
void List<T>::init()
{

}
// 清除所有节点
template <typename T>
int List<T>::clear()
{
    return 0;
}
// 复制列表中自位置P起的n项
template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n)
{

}
// 有序列表区间归并
template <typename T>
void List<T>::merge(ListNodePosi(T) &, int, List<T> &, ListNodePosi(T), int)
{

}
// 对从p开始的连续n个节点归并排序
template <typename T>
void List<T>::mergeSort(ListNodePosi(T) &, int)
{

}
// 对从p开始的连续n个节点选择排序
template <typename T>
void List<T>::selectionSort(ListNodePosi(T), int)
{

}
// 对从p开始的连续n个节点插入排序
template <typename T>
void List<T>::insertionSort(ListNodePosi(T), int)
{

}

// 构造函数
// 默认
template <typename T>
List<T>::List()
{

}
// 整体复制列表L
template <typename T>
List<T>::List(const List<T> &L)
{

}
// 复制列表L中第r项起的n项
template <typename T>
List<T>::List(const List<T> &L, Rank r, Rank n)
{

}
// 复制列表中自位置p起的n项
template <typename T>
List<T>::List(ListNodePosi(T) p, int n)
{

}
// 析构函数
// 释放包含头尾哨兵在内的所有节点
template <typename T>
List<T>::~List()
{

}
// 只读访问接口
// 规模
template <typename T>
int List<T>::size() const
{
    return 0;
}
// 判空
template <typename T>
bool List<T>::empty() const
{
    return true;
}
// 重载[]支持寻秩访问
template <typename T>
T &List<T>::operator[](Rank r) const
{
    return T();
}
// 首节点位置
template <typename T>
ListNodePosi(T) List<T>::first() const
{
    return NULL;
}
// 末节点位置
template <typename T>
ListNodePosi(T) List<T>::last() const
{
    return NULL;
}
// 判断位置p是否对外合法
template <typename T>
bool List<T>::valid(ListNodePosi(T) p) const
{
    return true;
}
// 判断列表是否已经有序
template <typename T>
int List<T>::disordered() const
{
    return 0;
}
// 无序列表查找
template <typename T>
ListNodePosi(T) List<T>::find(const T &e) const
{
    return NULL;
}
// 无序区间查找
template <typename T>
ListNodePosi(T) List<T>::find(const T &e, int n, ListNodePosi(T) p) const
{
    return NULL;
}
// 有序列表查找
template <typename T>
ListNodePosi(T) List<T>::search(const T &e)
{
    return NULL;
}
// 有序区间查找
template <typename T>
ListNodePosi(T) List<T>::search(const T &e, int n, ListNodePosi(T) p) const
{
    return NULL;
}
// 在p及其n-1个后继中选出最大者
template <typename T>
T List<T>::selectMax(ListNodePosi(T) p, int n)
{

}
// 整体最大者
template <typename T>
T List<T>::selectMax()
{
    return T();
}
// 可写访问接口
// 将e当作首节点插入
template <typename T>
void List<T>::insertAsFirst(const T &e)
{

}
// 将e当作末节点插入
template <typename T>
void List<T>::insertAsLast(const T &e)
{

}
// 将e当作p的前驱插入
template <typename T>
void List<T>::insertAsPred(ListNodePosi(T) p, const T &e)
{

}
// 将e当作p的后继插入
template <typename T>
void List<T>::insertAsSucc(ListNodePosi(T) p, const T &e)
{

}
// 删除合法位置p处的节点 返回被删除的节点
template <typename T>
T List<T>::remove(ListNodePosi(T) p)
{
    return T();
}
// 全列表归并
template <typename T>
void List<T>::merge(List<T> &L)
{

}
// 列表区间排序
template <typename T>
void List<T>::sort(ListNodePosi(T) s, int n)
{

}
// 列表整体排序
template <typename T>
void List<T>::sort()
{

}
// 无序去重
template <typename T>
int List<T>::deduplicate()
{

}
// 有序去重
template <typename T>
int List<T>::uniquify()
{

}
// 前后逆置
template <typename T>
void List<T>::reverse()
{

}
// 遍历
// 函数指针遍历
template <typename T>
void List<T>::traverse(void (*pfun)(T &val))
{

}
// 函数对象遍历
template <typename VST>
void traverse(VST &funobj)
{

}
};

int main(int argc, char* argv[])
{
    MYSTL::List<int> l;
    return 0;
}