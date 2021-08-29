/*
 * @lc app=leetcode.cn id=26 lang=cpp
 *
 * [26] 删除有序数组中的重复项
 */

// @lc code=start
class Solution {
public:
    // 1
    // 1 1
    // 1 1 2
    // 1 1 1 1 2 2 3 3 4 4
    int removeDuplicates(vector<int>& nums) {
        int len = 0;
        for (size_t i = 0; i < nums.size(); ++i) {
            if (nums[i] != nums[len]) {
                ++len;
                nums[len] = nums[i];
            }
        }
        return nums.size() > 0 ? ++len : 0;
    }
};
// @lc code=end

