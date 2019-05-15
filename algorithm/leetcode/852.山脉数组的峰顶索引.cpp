/*
 * @lc app=leetcode.cn id=852 lang=cpp
 *
 * [852] 山脉数组的峰顶索引
 */
class Solution {
public:
    int peakIndexInMountainArray(vector<int>& A) {
        if (A.size() < 3)
            return -1;
        int s = 0;
        int e = A.size() - 1;
        while (s <= e)
        {
            int m = e - (e - s) / 2;
            if (A[m - 1] < A[m] && A[m] > A[m + 1])
                return m;
            else if (A[m] > A[m - 1])
                s = m + 1;
            else
                e = m - 1;
        }
        return -1;
    }
};

