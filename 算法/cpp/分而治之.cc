
#include <algorithm>
#include <vector>
#include <cmath>
#include <cfloat>


// 最大连续子序列和
// greatest slice
// 蛮力策略
int gs_BF(int A[], int n)
{
    int gs = A[0];
    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n; ++j)
        {
            int s = 0;
            for (int k = i; k <= j; ++k)
                s += A[k];
            if (gs < s)
                gs = s;
        }
    }
    return gs;
}

// 增量策略
int gs_IC(int A[], int n)
{
    int gs = A[0];
    for (int i = 0; i < n; ++i)
    {
        int s = 0;
        for (int j = i; j < n; ++j)
        {
            s += A[j];
            if (gs < s)
                gs = s;
        }
    }
    return gs;
}

// 分而治之
int gs_DC(int A[], int lo, int hi)
{
    if (hi - lo < 2)
        return A[lo];   // TODO
    int mi = (lo + hi) / 2;
    int gsL = A[mi - 1], sL = 0, i = mi;
    while (lo < i--)
        if (gsL < (sL += A[i]))
            gsL = sL;
    int gsR = A[mi], sR = 0, j = mi-1;
    while (++j < hi)
        if (gsR < (sR += A[j]))
            gsR = sR;
    return max(gsL + gsR, max(gs_DC(A,lo,mi), gs_DC(A,mi,hi)));
}

// 减而治之
int gs_LS(int A[], int n)
{
    int gs = A[0], s = 0, i = n, j = n;
    while (0 < i--) // 从后往前剪掉不符合条件的 TODO从前往后?
    {
        s += A[i];
        if (gs < s)
            gs = s;
        if (s <= 0)
        {
            s = 0;
            j = i;
        }
    }
    return gs;
}


// void mergeSort(int lo, int hi)
// {
//     if (hi - lo < 2)
//         return;
//     int mi = (lo + mi) >> 1;
//     mergeSort(lo, mi);
//     mergeSort(mi, hi);
//     merge(lo, mi, hi);
// }

// 减而治之+分而治之
// 找出无序数组A中第k大的元素M
// 要求:<=线性时间复杂度
// 划分数组A为n个区间(每个区间长度为Q),对每个区间执行划分算法,共n/Q个区间
// 假设每个区间长度为p,取每个有序区间的p/2位置的值
// 找出n/Q个区间中所有p/2位置数字的中位数 以该数进行一次划分
// 最终得出 L(大于M的区间) E(等于M的区间) G(小于M的区间)

// int linearSelect(vector<int>& A, int k)
// {
//     // 递归基
//     if ()
//     {
//         return trivialSelect(A, k);
//     }

// }

////////////////////////////////////////////////////////////////////////////
template<typename type>
void Swap(Type &x, Type &y)
{
    Type tmp = x;
    x = y;
    y = tmp;
}
template<typename Type>
void BubbleSort(Type A[], int p, int r)
{
    bool exchange;
    for (int i = p; i < r; ++i)
    {
        exchange = false;
        for (int j = 0; j <= r-i; ++j)
        {
            if (A[j] < A[j-1])
            {
                Swap(A[j], A[j-1]);
                exchange = true;
            }
        }
        if (false == exchange)
            break;
    }
}

template<typename Type>
int Partition(Type A[], int p, int r, Type x)
{
    int i = p-1;
    int j = r+1;
    while (true)
    {
        while (A[++i] < x && i < r);
        while (A[--j] > x);
        if (i >= j)
            break;
        Swap(A[i]. A[j]);
    }
    return j;
}

template<typename Type>
Type Select(Type A[], int p, int r, int k)
{
    if (r-p<75)
    {
        BubbleSort(A,p,r);
        return A[p+k-1];
    }

    // 分组排序
    for (int i = 0; i <= (r-p-4)/5; ++i)
    {
        BubbleSort(A, p+5*i, p+5*i+4);
        Swap(A[p+5*i+2], A[p+i]);
    }

    // 递归分治
    // 找中位数的中位数
    Type x = Select(A, p, p+(r-p-4)/5, (r-p-4)/10);
    i = Partition(A, p, r, x);
    int j = i - p + 1;
    // 减治
    if (k <= j)
    {
        return Select(A, p, i, k);
    }
    else
    {
        return Select(A, i+1, r, k-j);
    }
    
}

////////////////////////////////////////////////////////////////////////////
int findKthLargest(vector<int> &nums, int k)
{
    return findKthLargest(nums, 0, nums.size()-1, nums.size()-k);
}
int findKthLargest(vector<int> &nums, int start, int end, int k)
{
    if (start > end)
        return -1;
    int pivot = nums[end];
    int left = start;
    // 对整个序列进行一次划分
    for (int i = start; i < end; ++i)
    {
        if (nums[i] <= pivot)
        {
            swap(nums, left, i);
            ++left;
        }
    }
    swap(nums, left, end);
    if (left == k)
    {
        return nums[left];
    }
    else if (left < k)
    {
        return findKthLargest(nums, left+1, end, k);
    }

    return findKthLargest(nums, start, left-1, k);
}
void swap(vector<int> &A, int i, int j)
{
    int tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

// diameter of a binary tree
// 求二叉树的直径分两种情况 
// 经过根节点 则为左右子树高度和加1
// 不经过根节点 左右子树中直径的最大值
int diameter(TreeNode *root)
{
    if (NULL == root)
        return 0;
    
    int leftHight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    int leftDia = diameter(root->left);
    int rightDia = diameter(root->right);

    int maxSubDia = max(leftDia, rightDia);
    return max(maxSubDia, leftHight+rightHeight+1);
}
int getHeight(TreeNode *root)
{
    if (NULL == root)
        return 0;
    return 1 + max(getHeight(root->left), getHeight(root->right));
}
