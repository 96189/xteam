/*
 * @lc app=leetcode.cn id=832 lang=cpp
 *
 * [832] 翻转图像
 */
class Solution {
public:
    vector<vector<int>> flipAndInvertImage(vector<vector<int>>& A) {
        vector<vector<int>> newA(A);
        // 行翻转 并反转
        for (auto &line : newA)
        {
            reverse(line.begin(), line.end());
            transform(line.begin(), line.end(), line.begin(), [](int &a) -> int { return a ^= 1; });
        }
        return newA;
    }
};

