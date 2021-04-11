/*
 * @lc app=leetcode.cn id=219 lang=cpp
 *
 * [219] 存在重复元素 II
 */

// @lc code=start
class Solution {
public:
    bool containsNearbyDuplicate(vector<int>& nums, int k) {
        map<int, int> num2index;
        for (size_t i = 0; i < nums.size(); ++i) {
            auto it = num2index.find(nums[i]);
            if (it != num2index.end()) {
                if (i - num2index[nums[i]] <= k) {
                    return true;
                }
            }
            num2index[nums[i]] = i;
        }
        return false;
    }
};
// @lc code=end

