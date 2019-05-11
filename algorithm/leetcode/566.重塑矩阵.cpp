/*
 * @lc app=leetcode.cn id=566 lang=cpp
 *
 * [566] 重塑矩阵
 */
class Solution {
public:
    vector<vector<int>> matrixReshape(vector<vector<int>>& nums, int r, int c) {
        if (nums.empty())
            return nums;
        if ((int)(nums.size() * nums[0].size()) < (r * c))
            return nums;

        int oldRow = 0;
        int oldCol = 0;
        int oldColMax = nums[0].size();
        vector<vector<int>> newMatrix;
        for (int row = 0; row < r; ++row)
        {
            vector<int> newRow;
            for (int col = 0; col < c; ++col)
            {
                newRow.push_back(nums[oldRow][oldCol++]);
                if (oldCol >= oldColMax)
                {
                    ++oldRow;
                    oldCol = 0;
                }
            }
            newMatrix.push_back(newRow);
        }
        return newMatrix;
    }
};

