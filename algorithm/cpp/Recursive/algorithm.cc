#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// 递归
// n = 0, f(n) = 0
// n = 1, f(n) = 1
// n >=2, f(n) = f(n-1) + f(n-2)
long fib(int n)
{
    // 出口条件
    if (0 == n)
    {
        return 0;
    }
    else if (1 == n)
    {
        return 1;
    }
    else 
    {
        // 递归(数学归纳)
        // go + solve + back
        return (fib(n - 1) + fib(n - 2));
    }
}

// n = 0, f(n) = 1
// n > 0, f(n) = n * f(n-1)
long factorial(int n)
{
    // 出口条件
    if (0 == n)
    {
        return 1;
    }
    else
    {
        // 递归(数学归纳)
        // go + solve + back
        return (n * factorial(n - 1));
    }
}

// n 个数
// x from
// z to
// y tmp
void HanoiTower(int n, char x, char y, char z)
{
    // 出口条件(基本用例)
    // 数学归纳法
    if (1 == n)
    {
        printf("No.%d : %c -> %c\n", n, x, z);
    }
    else 
    {
        // x(n-1) y
        // 从x移动[1,n-1]到y
        HanoiTower(n-1, x, z, y);
        // x(1) z
        // 从x移动[n,n]到z
        HanoiTower(1, x, y, z);
        // y(n-1) z
        // 从y移动[1,n-1]到z
        HanoiTower(n-1, y, x, z);
        
    }
}

int length(const char* str)
{
    // 出口条件
    if (*str == '\0')
    {
        return 0;
    }
    else 
    {
        // 递归(归纳)
        // go + solve + back
        return length(str++) + 1;
    }
}

class Node
{
public:
	int val_;
	Node* next_;

	Node()
	{

	}
	Node(int val) : val_(val)
	{
		next_ = NULL;
	}
};

Node* CreateList(int n)
{
	Node* head = new Node(10);
	for (int i = 1; i < n; ++i)
	{
		Node* pCur = new Node(i);
		pCur->next_ = head->next_;
		head->next_ = pCur;
	}
	return head;
}
void IteratorList(Node* head)
{
	Node* pCur = head;
	while (pCur != NULL)
	{
		printf("%d\n", pCur->val_);
		pCur = pCur->next_;
	}
}
// go -> back -> solve
Node* Reverse(Node* pCur)
{
    Node* phead = NULL;
    // 出口条件
    if (NULL == pCur->next_)
    {
        return pCur;
    }

    // go步进(递)到最后节点
    // 减小问题规模
    phead = Reverse(pCur->next_);

    // back(归)
    // solve(逻辑处理)
    pCur->next_->next_ = pCur;
    pCur->next_ = NULL;
    return phead;
}

void PrintCArray(int arr[], int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf("%d -> ", arr[i]);
    }
    printf("\n");
}

// arr
// [left1, right1]
// [left2, right2]
void Copy(int arr[], int left1, int right1, int left2, int right2, int assist[], int start, int end)
{
    int index = start; 
    while (left1 <= right1 && left2 <= right2)
    {
        int min = arr[left1] < arr[left2] ? arr[left1++] : arr[left2++];
        assist[index++] = min;
    }
    while (left1 <= right1)
    {
        assist[index++] = arr[left1++];
    }
    while (left2 <= right2)
    {
        assist[index++] = arr[left2++];
    }
    // 排好序的序列拷贝到原序列对应的位置
    for (index = start; index <= end; ++index)
    {
        arr[index] = assist[index];
    }
}

// go -> back -> solve
// arr [start, end]
void Merge(int arr[], int start, int end, int assist[])
{
    // 出口条件(分解问题规模到最小)
    if (start == end)
    {
        return ;
    }
    int mid = (start + end) / 2;
    int left1 = start;
    int right1 = mid;
    int left2 = mid + 1;
    int right2 = end;

    // 递(步进)go
    Merge(arr, left1, right1, assist);
    Merge(arr, left2, right2, assist);

    // 归(处理)back
    // solve(逻辑处理)
    Copy(arr, left1, right1, left2, right2, assist, start, end);
}
void MergeSort(int arr[], int len, int assist[])
{
    Merge(arr, 0, len - 1, assist);
}

void Swap(int& v1, int& v2)
{
    int t = v1;
    v1 = v2;
    v2 = t;
}

int Partition(int arr[], int start, int end)
{
    int value = arr[start];
    while (start != end)
    {
        while (start != end && arr[start] < value)
        {
            ++start;
        }
        while (start != end && arr[end] > value)
        {
            --end;
        }
        if (start == end)
        {
            break;
        }
        Swap(arr[start], arr[end]);
    }
    return start;
}

// solve -> go -> back
void DoQuickSort(int arr[], int start, int end)
{
    // 出口条件
    if (start == end)
    {
        return;
    }

    // solve(逻辑处理)
    int base = Partition(arr, start, end);
    
    // 递(go步进)
    DoQuickSort(arr, start, base);
    DoQuickSort(arr, base + 1, end);

    // 归(back)
    // nothing
}
void QuickSort(int arr[], int len)
{
    DoQuickSort(arr, 0, len - 1);
}

// solve -> go -> back
void Find(int arr[], int start, int end, int& index, int value)
{ 
    int mid = (start + end) / 2;
    // 出口条件
    if (start >= end)
    {
        return;
    }

    // solve(逻辑处理)
    if (arr[mid] == value)
    {
        index = mid;
    }

    // go步进减小问题规模
    Find(arr, mid + 1, end, index, value);
    Find(arr, start, mid, index, value);

    // back
    // nothing
}

// 快速排序
// 二分查找
// solve -> go -> back
// 在递的过程中解决问题,局部逻辑处理完成整体逻辑处理

// 链表逆序
// 归并排序
// go -> back -> solve
// 在归的过程中解决问题,问题的解决需要达到最小规模

int main(int argc, char* argv[])
{
    // HanoiTower(3, 'X', 'Y', 'Z');
    int arr[] = { 5, 3, 2, 4, 1, 7, 6, 8, 9, 0 };
    int len = sizeof(arr) / sizeof(arr[0]);
    // int *assist = (int *)malloc(len * sizeof(int));
    // memset(assist, 0, sizeof(int) * len);
    // MergeSort(arr, len, assist);
    // PrintCArray(assist, len);
    // free(assist);
    // QuickSort(arr, len);
    // // Partition(arr, 0, len-1);
    // PrintCArray(arr, len);
    // int index = -1;
    // Find(arr, 0, len - 1, index, 7);
    // if (index != -1)
    // {
    //     printf("index=%d\n",index);
    //     printf("%d\n", arr[index]);
    // }
    Node* h = CreateList(10);
    //IteratorList(h);
    Node* newh = Reverse(h);  
    IteratorList(newh);
    return 0;
}