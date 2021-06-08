/*
 * @lc app=leetcode.cn id=217 lang=cpp
 *
 * [217] 存在重复元素
 */

// @lc code=start
class Solution {
public:
    // bool containsDuplicate(vector<int>& nums) {
    //     std::sort(nums.begin(), nums.end());
    //     for (size_t i = 1; i < nums.size(); ++i) {
    //         if (nums[i] == nums[i-1]) {
    //             return true;
    //         }
    //     }
    //     return false;
    // }
    bool containsDuplicate(vector<int>& nums) {
        std::set<int> s;
        for (size_t i = 0; i < nums.size(); ++i) {
            auto it = s.find(nums[i]);
            if (it != s.end()) {
                return true;
            }
            s.insert(nums[i]);
        }
        return false;
    }
};
// @lc code=end

