#ifndef _LIST_H_
#define _LIST_H_
#include <cstddef>
namespace MYSTL 
{

typedef int Rank;
#define ListNodePosi(T) ListNode<T>*

template <typename T>
class ListNode 
{
public:
    T data;
    // 前驱
    ListNodePosi(T) pred;
    // 后继
    ListNodePosi(T) succ;

    // 构造函数 针对header和trailer的构造
    ListNode() 
    {
        data = T();
        pred = succ = NULL;
    }
    ListNode(T e, ListNodePosi(T) p = NULL, ListNodePosi(T) s = NULL)
        : data(e), pred(p), succ(s) {}

// 操作接口
    // 读取数据
    T& getData() 
    {
        return data;
    } 
    void setData(T val)
    {
        data = val;
    }
    // 在当前节点之前插入
    ListNodePosi(T) insertAsPred(const T& e)
    {
        ListNodePosi(T) pCur = new ListNode(e, this->pred, this);
        if (this->pred)
        {
            this->pred->succ = pCur;
        }
        this->pred = pCur;
        return pCur;
    }
    // 在当前节点之后插入
    ListNodePosi(T) insertAsSucc(const T& e)
    {
        ListNodePosi(T) pCur = new ListNode(e, this, this->succ);
        if (this->succ)
        {
            this->succ->pred = pCur;
        }
        this->succ = pCur;
        return pCur;
    }
};

// 双向链表定义
// 双向链表结束不以NULL为标记
// 尾端存在trailer哨兵节点
template <typename T>
class List 
{
private:
    int _size;
    ListNodePosi(T) header;
    ListNodePosi(T) trailer;
protected:
    // 创建列表时的初始化
    void init();
    // 清除所有节点
    int clear();
    // 复制列表中自位置P起的n项
    void copyNodes(ListNodePosi(T) p, int n);
    // 有序列表区间归并
    void merge(ListNodePosi(T)& , int , List<T>& ,ListNodePosi(T), int );
    // 对从p开始的连续n个节点归并排序
    void mergeSort(ListNodePosi(T)& , int);
    // 对从p开始的连续n个节点选择排序
    void selectionSort(ListNodePosi(T), int);
    // 对从p开始的连续n个节点插入排序
    void insertionSort(ListNodePosi(T), int);
public:
// 构造函数
    // 默认
    List();
    // 整体复制列表L
    List(const List<T>& L);
    // 复制列表L中第r项起的n项
    List(const List<T>& L, Rank r, Rank n);
    // 复制列表中自位置p起的n项
    List(ListNodePosi(T) p, int n);
// 析构函数
    // 释放包含头尾哨兵在内的所有节点
    ~List();
// 只读访问接口
    // 规模
    int size() const;
    // 判空
    bool empty() const;
    // 重载[]支持寻秩访问
    T& operator[](Rank r) const;
    // 首节点位置
    ListNodePosi(T) first() const;
    // 末节点位置
    ListNodePosi(T) last() const;
    // 判断位置p是否对外合法
    bool valid(ListNodePosi(T) p) const;
    // 判断列表是否已经有序
    int disordered() const;
    // 无序列表查找
    ListNodePosi(T) find(const T& e) const;
    // 无序区间查找
    ListNodePosi(T) find(const T& e, int n, ListNodePosi(T) p) const;
    // 有序列表查找
    ListNodePosi(T) search(const T& e);
    // 有序区间查找
    ListNodePosi(T) search(const T& e, int n, ListNodePosi(T) p) const;
    // 在p及其n-1个后继中选出最大者
    T selectMax(ListNodePosi(T) p, int n);
    // 整体最大者
    T selectMax();
// 可写访问接口
    // 将e当作首节点插入
    void insertAsFirst(const T& e);
    // 将e当作末节点插入
    void insertAsLast(const T& e);
    // 将e当作p的前驱插入
    void insertAsPred(ListNodePosi(T) p, const T& e);
    // 将e当作p的后继插入
    void insertAsSucc(ListNodePosi(T) p, const T& e);
    // 删除合法位置p处的节点 返回被删除的节点
    T remove(ListNodePosi(T) p);
    // 全列表归并
    void merge(List<T>& L);
    // 列表区间排序
    void sort(ListNodePosi(T) s, int n);
    // 列表整体排序
    void sort();
    // 无序去重
    int deduplicate();
    // 有序去重
    int uniquify();
    // 前后逆置
    void reverse();
// 遍历
    // 函数指针遍历
    void traverse(void (*visit)(T& val));
    // 函数对象遍历
    template <typename VST>
    void traverse(VST& funobj);
}; // List

}; // namespace MYSTL

#endif