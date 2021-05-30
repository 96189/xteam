/*
 * @lc app=leetcode.cn id=167 lang=cpp
 *
 * [167] 两数之和 II - 输入有序数组
 */

// @lc code=start
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int l = 0;
        int r = numbers.size() - 1;
        std::vector<int> result;
        while (r > l) {
            int sum = numbers[r] + numbers[l];
            if (sum == target) {
                result.push_back(l+1);
                result.push_back(r+1);
                break;
            } else if (sum > target) {
                --r;
            } else {
                ++l;
            }
        }
        return result;
    }
};
// @lc code=end

