/*
 * @lc app=leetcode.cn id=976 lang=cpp
 *
 * [976] 三角形的最大周长
 */
class Solution {
public:
    int largestPerimeter(vector<int>& A) {
        sort(A.begin(), A.end(), [](const int a, const int b) -> bool {return a > b;});
        for (size_t i = 2; i < A.size(); ++i)
        {
            if (A[i] + A[i-1] > A[i-2])
                return A[i] + A[i-1] + A[i-2];
        }
        return 0;
    }
};

