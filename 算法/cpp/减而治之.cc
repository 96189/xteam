#include <vector>

using namespace std;

// 二分查找
int binSearch(int S[], const int e, int lo, int hi)
{
    while (lo < hi)
    {
        int mi = (lo + hi) >> 1;
        e < S[mi] ? hi = mi : lo = mi + 1;  // 每次减掉1/2
    }
    return --lo;
}

// 选择排序(没有最好的情况-输入不敏感-offline算法数据一次性可以给定)

// 堆排序
// 优化选择排序的选择过程O(n^2)到O(logn)


// 插入排序(逆序对-输入敏感-存在最好的情况-online算法数据不能一次性给定的时候)

// 快速排序的划分函数-无序序列中选取第k大的元素
void quickSelect(vector<int> A, int k)
{
    for (int lo = 0, hi = A.size()-1; lo < hi; )
    {
        int i = lo, j = hi;
        int pivot = A[lo];
        while (i < j)
        {
            while (i < j && pivot <= A[j])
                --j;
            A[i] = A[j];
            while (i < j && A[i] <= pivot)
                ++i;
            A[j] = A[i];
        }
        A[i] = pivot;
        if (k <= i)
            hi = i - 1;
        if (i <= k)
            lo = i + 1;
    }
}

// 找众数
int majCandidate(vector<int> A)
{
    int maj;
    for (int c = 0, i = 0; i < A.size(); ++i)
    {
        if (c == 0)
        {
            maj = A[i];
            c = 1;
        }
        else
        {
            maj == A[i] ? ++c : --c;
        }
    }
    return maj;
}

