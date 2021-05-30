/*
 * @lc app=leetcode.cn id=16 lang=cpp
 *
 * [16] 最接近的三数之和
 */

// @lc code=start
class Solution {
public:
    // 1、3个数求和
    // 2、和目标值做差值
    // 3、取差值最小的 理想情况差值为0,转换为三个数之和
    int threeSumClosest(vector<int>& nums, int target) {
        int ans = target;
        int diff = 0x7fffffff;
        std::sort(nums.begin(), nums.end());
        for (int i = 0; i < nums.size(); ++i) {
            if (i > 0 && nums[i] == nums[i-1]) {
                continue;
            }
            int l = i + 1;
            int r = nums.size() - 1;
            while (r > l) {
                int sum = nums[i] + nums[l] + nums[r];
                int newdiff = abs(sum - target);
                if (newdiff < diff) {
                    ans = sum;
                    diff = newdiff;
                }
                if (sum == target) {
                    ++l;
                    --r;
                } else if (sum > target) {
                    --r;
                } else {
                    ++l;
                }
            }
        }
        return ans;
    }
};
// @lc code=end

