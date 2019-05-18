/*
 * @lc app=leetcode.cn id=961 lang=cpp
 *
 * [961] 重复 N 次的元素
 */
class Solution {
public:
    int repeatedNTimes(vector<int>& A) {
        sort(A.begin(), A.end());
        return A[A.size() / 2] == A[A.size() - 1] ? A[A.size() - 1] : A[A.size() / 2 - 1];
    }
};

