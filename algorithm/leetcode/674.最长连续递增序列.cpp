/*
 * @lc app=leetcode.cn id=674 lang=cpp
 *
 * [674] 最长连续递增序列
 */
class Solution {
public:
    int findLengthOfLCIS(vector<int>& nums) {
        if (nums.empty())
            return 0;
        int maxCnt = 0;
        int cnt = 1;
        int size = nums.size();
        for (int i = 0; i < size - 1; ++i)
        {
            if (nums[i] < nums[i + 1])
            {
                ++cnt;
            }
            else
            {
                maxCnt = max(cnt, maxCnt);
                cnt = 1;
            }
        }
        return max(cnt, maxCnt);
    }
};

