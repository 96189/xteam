/*
 * @lc app=leetcode.cn id=896 lang=cpp
 *
 * [896] 单调数列
 */
class Solution {
public:
    bool isMonotonic(vector<int>& A) {
        // if (A.size() <= 1)
        //     return true;
        // // 检测当前数列起始处的增减性
        // int order = 0; // 假设数列元素间相等
        // size_t i;
        // for (i = 1; i < A.size(); ++i)
        // {
        //     if (A[i] - A[i - 1] != 0)
        //     {
        //         order = A[i] - A[i - 1] > 0 ? 1 : -1;
        //         break;
        //     }
        // }
        // if (i == A.size())
        //     return true;
        // for (; i < A.size(); ++i)
        // {
        //     if (A[i] - A[i - 1] != 0)
        //     {
        //         int curOrder = A[i] - A[i - 1] > 0 ? 1 : -1;
        //         if (curOrder != order)
        //             return false;
        //     }
        // }
        // return true;
        bool inc = true;
        bool dec = true;
        int n = A.size();
        for (int i = 0; i < n - 1; ++i)
        {
            if (A[i] > A[i + 1])
                inc = false;
            if (A[i] < A[i + 1])
                dec = false;
        }
        return inc || dec;
    }
};

