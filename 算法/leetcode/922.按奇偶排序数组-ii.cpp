/*
 * @lc app=leetcode.cn id=922 lang=cpp
 *
 * [922] 按奇偶排序数组 II
 */
class Solution {
public:
    vector<int> sortArrayByParityII(vector<int>& A) {
        int evenIdx = 0;
        int oddIdx = 1;
        vector<int> newA(A.size(), 0);
        for (size_t i = 0; i < A.size(); ++i)
        {
            if (A[i] % 2 == 0)
            {
                newA[evenIdx] = A[i];
                evenIdx += 2;
            }
            else
            {
                newA[oddIdx] = A[i];
                oddIdx += 2;
            }
        }
        return newA;
    }
};

