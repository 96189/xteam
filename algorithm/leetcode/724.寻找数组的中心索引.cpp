/*
 * @lc app=leetcode.cn id=724 lang=cpp
 *
 * [724] 寻找数组的中心索引
 */
class Solution {
public:
    int pivotIndex(vector<int>& nums) {
        if (nums.size() < 3)
            return -1;
        vector<int> pivots;
        vector<int> sums;
        sums.push_back(nums[0]);
        for (size_t i = 1; i < nums.size(); ++i)
        {
            sums.push_back(nums[i] + sums[i - 1]);
        }
        if (sums[sums.size() - 1] - sums[0] == 0)
        {
            pivots.push_back(0);
        }
        if (sums[sums.size() - 2] == 0)
        {
            pivots.push_back(sums.size() - 1);
        }
        for (size_t idx = 1; idx <= nums.size() - 2; ++idx)
        {
            int prevSum = sums[idx - 1];
            int lastSum = sums[sums.size() - 1] - sums[idx];
            if (prevSum == lastSum)
            {
                pivots.push_back(idx);
            }
        }
        if (pivots.empty())
            return -1;
        sort(pivots.begin(), pivots.end());
        return pivots[0];
    }
};

