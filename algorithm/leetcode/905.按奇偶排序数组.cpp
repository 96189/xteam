/*
 * @lc app=leetcode.cn id=905 lang=cpp
 *
 * [905] 按奇偶排序数组
 */
class Solution {
public:
    vector<int> sortArrayByParity(vector<int>& A) {
        vector<int> newA(A.size(), 0);
        int s = 0;
        int e = A.size() - 1;
        for (auto &val : A)
        {
            if (val % 2 != 0)
            {
                newA[e--] = val;
            }
            else
            {
                newA[s++] = val;
            }
        }
        return newA;
    }
};

