/*
 * @lc app=leetcode.cn id=941 lang=cpp
 *
 * [941] 有效的山脉数组
 */
class Solution {
public:
    bool validMountainArray(vector<int>& A) {
        int s = 0;
        int e = A.size() - 1;
        // 山峰数组至少有3个元素
        if (e < 2)
            return false;
        // 从左向右找山峰位置
        int ss = s;
        while (s < e)
        {
            if (A[s] < A[s + 1])
                ++s;
            else
                break;
        }
        if (ss == s)
            return false;
        // 从右向左找山峰位置
        int ee = e;
        while (e > 1)
        {
            if (A[e - 1] > A[e])
                --e;
            else
                break;
        }
        if (ee == e)
            return false;

        return s == e;
    }
};

