/*
 * @lc app=leetcode.cn id=867 lang=cpp
 *
 * [867] 转置矩阵
 */
class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& A) {
        vector<vector<int>> newA;
        for (size_t col = 0; col < A[0].size(); ++col)
        {
            vector<int> newRow;
            for (size_t row = 0; row < A.size(); ++row)
            {
                newRow.push_back(A[row][col]);
            }
            newA.push_back(newRow);
        }
        return newA;
    }
};

