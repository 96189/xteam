/*
 * @lc app=leetcode.cn id=15 lang=cpp
 *
 * [15] 三数之和
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int target = 0;
        // 排序
        std::sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); ++i) {
            // 被固定的数字
            if (i > 0 && nums[i] == nums[i-1]) {
                continue;
            }
            int s = i + 1;
            int e = nums.size() - 1;
            while (e > s) {
                int sum = nums[i] + nums[s] + nums[e];
                if (target == sum) {
                    result.push_back(vector<int>{nums[i], nums[s], nums[e]});
                    ++s;
                    // 已经查找过的数字
                    while (e > s && nums[s] == nums[s-1]) {
                        ++s;
                    }
                    --e;
                    // 已经查找过的数字
                    while (e > s && nums[e] == nums[e+1]) {
                        --e;
                    }
                } else if (target < sum) {
                    --e;
                } else {
                    ++s;
                }
            }
        }
        return result;
    }
};
// @lc code=end

