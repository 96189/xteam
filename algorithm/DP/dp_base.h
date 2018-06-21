#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
// https://www.zhihu.com/question/23995189
// https://blog.csdn.net/u013445530/article/details/45645307
// https://blog.csdn.net/u013309870/article/details/75193592
// https://blog.csdn.net/baidu_28312631/article/details/47418773
// https://www.cnblogs.com/jycboy/p/6413947.html
// 动态规划
// 应用场景:原问题可以转化为子问题,每一个阶段存在多个状态,存在重叠的子问题
// 本质:
// 状态的定义(子问题)
// 状态转移方程的定义

// 初始状态
// 递推公式

// 硬币组合
// Q: 如果我们有面值为1元、3元和5元的硬币若干枚，如何用最少的硬币凑够11元？
// -> 如何用最少的硬币凑够i元(i<11)？
// 阶段 i = 0, 状态 money(0) = 0                            // 初始状态,凑够0元的硬币需要0个硬币
// 阶段 i = 1, 状态 money(1) = money(1-1) + 1 = 1           //
// 阶段 i = 2, 状态 money(2) = money(2-1) + 1 = 2           //
// 阶段 i = 3, 状态 money(3) = money(3-1) + 1 = 3           // 本阶段存在2种状态
//                 money(3) = money(3-3) + 1 = 1           //
// ...
// 阶段 i = 5, 状态 money(5) = money(5-5) + 1 = 1
//                 money(5) = money(5-3) + 1 = 3
//                 money(5) = money(5-1) + 1 = 3
// ...
// 阶段 i = 10,状态 money(10) = money(10-5) + 1 = 2
//
// 阶段i=3时 状态money(3)存在最优解 money(3) = min{money(3-3) + 1, money(3-1) + 1}
// 设y = value[x],用最少的y凑够i元
// 状态转移方程为: money(i) = money(i - y) + 1 其中i-y >= 0,y表示value中第x个硬币值

int MinCoinReachAimMoney(const int aim)
{
    int coins[3] = {1, 2, 5};
    int *dp = (int *)malloc((aim + 1) * sizeof(int));
    assert(dp);
    // 用coins中的硬币凑aim元
    // 如果aim<coins[0],那么无解,0表示无解
    for (int i = 0; i < coins[0]; ++i)
    {
        dp[i] = 0;
    }

    for (int i = coins[0]; i <= aim; ++i)
    {
        for (int index = 0; index < 3; ++index)
        {
            if (i >= coins[index])
            {
                dp[i] = dp[i - coins[index]] + 1;
            }
        }
    }
    int num = dp[aim];
    free(dp);
    return num;
}

// 数塔问题
void InitTower(int tower[], const int N)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j <= i; ++j)
        {
            *(tower + N * i + j) = rand() % 15;
        }
    }
}
void ShowTower(int tower[], const int N)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%2d ", *(tower + N * i + j));
        }
        printf("\n");
    }
    printf("\n");
}

void PrintPath(int tower[], int dp[], int N)
{
    int j = 0;
    int value;
    printf(" -> %d ", tower[0]);
    for (int i = 1; i < N; ++i)
    {
        // dp和tower对应位置的差值是dp上一次的结果,则tower中对应上一次位置为选出的路径
        // dp[i-1][j] - tower[i-1][j]
        value = *(dp + N * (i - 1) + j) - *(tower + N * (i - 1) + j);
        // dp[i][j+1]
        if (value == *(dp + N * i + j + 1))
        {
            ++j;
        }
        // tower[i][j]
        printf(" -> %d ", *(tower + i * N + j));
    }
    printf("\n");
}
// 假设dp[i][j]表示从顶点(0,0)到点(i,j)的最大值
// 则有状态转移方程:
// 自顶向下
// dp[i][j] = dp[i][j] + max(dp[i-1][j-1], dp[j-1][j]), i >=1 j >=1
int MaxPathSumDown(int tower[], const int N)
{
    // 多分配一行一列空间,是保证状态转移方程的正确性,保证满足限制条件
    // 防止操作内存越界
    int dp[(N + 1) * (N + 1)];
    // 初始化整个dp矩阵为0
    memset(dp, 0, (N + 1) * (N + 1) * sizeof(int));
    // 填充tower到dp对应的位置
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            *(dp + (N + 1) * (i + 1) + (j + 1)) = *(tower + N * i + j);
        }
    }
    // 动态规划计算
    for (int i = 1; i <= N; ++i)
    {
        for (int j = 1; j <= i; j++)
        {
            *(dp + (N + 1) * i + j) = *(dp + (N + 1) * i + j) + std::max(*(dp + (N + 1) * (i - 1) + (j - 1)), *(dp + (N + 1) * (i - 1) + (j)));
        }
    }
    ShowTower(dp, N + 1);
    // 寻找最下层中的最大值
    int *last = (dp + (N + 1) * ((N + 1) - 1));
    int max = last[0];
    for (int i = 1; i < (N + 1); ++i)
    {
        if (last[i] > max)
        {
            max = last[i];
        }
    }
    return max;
}

// 自底向上
// dddp[i-1][j] = dp[i-1][j] + std::max(dp[i][j], dp[i][j+1]), i >= 1
int MaxPathSumUp(int tower[], const int N)
{
    int dp[N * N];
    // 填充tower到dp
    memcpy(dp, tower, N * N * sizeof(int));
    for (int i = N - 1; i >= 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            *(dp + N * (i - 1) + j) = *(dp + N * (i - 1) + j) + std::max(*(dp + N * i + j), *(dp + N * i + (j + 1)));
        }
    }
    ShowTower(dp, N);
    PrintPath(tower, dp, N);
    return dp[0];
}

// LIS(longest increasing subsequence)
// 最长非降序子序列
// 一个序列有N个数：A[1],A[2],…,A[N]，求出最长非降子序列的长度
// 假设i<n, 定义d(i)为序列中前i个数中以A[i]结尾的最长非降序子序列的长度
// 有序列: 2, 2, 5, 1, 1, 3, 4, 8, 6, 7
// i = 1, d(1) = 1,          (2)                        // 初始状态
// i = 2, d(2) = 2,          (2 2)
// i = 3, d(3) = 3,          (2 2 5)
// i = 4, d(4) = 3,          (2 2 5) (1)
// i = 5, d(5) = 3,          (2 2 5) (1 1)
// i = 6, d(6) = 3,          (2 2 5) (1 1 3)
// i = 7, d(7) = 4,          (2 2 5) (1 1 3 4)
// i = 8, d(8) = 5,          (2 2 5) (1 1 3 4 8)
// i = 9, d(9) = 5,          (2 2 5) (1 1 3 4 8) (6)
// i = 10, d(10) = 6         (2 2 5) (1 1 3 4 6 7) (8)

// d(i) = max{1, d(j) + 1}, j < i && A[j] <= A[i]

int LIS(int arr[], int n)
{
    int dp[n + 1];
    int maxSeq = 0;
    memset(dp, 0, (n + 1) * sizeof(int));
    // n个待计算的序列
    for (int i = 0; i < n; ++i)
    {
        // 当前序列长度至少为1
        dp[i] = 1;
        // 计算当前序列中的最长非降序子序列长度
        for (int j = 0; j < i; ++j)
        {
            if (arr[j] <= arr[i])
            {
                dp[i] = std::max(dp[i], dp[j] + 1);
            }
        }
        // 已计算出的最长长度和本次计算出的最长长度比较
        maxSeq = std::max(maxSeq, dp[i]);
    }
    return maxSeq;
}

// LCS
// 最长公共子序列

// 缓存 重叠子问题 记忆化
// 递推式求解(递归)
// fibonacci
// 每个阶段只有一个状态(递推)
// f(0) = 0
// f(1) = 1
// 状态转移方程
// f(n) = f(n-1) + f(n-2) n>=2

// Q: 如果我们有面值为2元、3元和5元的硬币若干枚，如何用最少的硬币凑够11元？
// Q: LIS时间复杂度为O(n^2),尝试优化