/*
 * @lc app=leetcode.cn id=18 lang=cpp
 *
 * [18] 四数之和
 */

// @lc code=start
class Solution {
public:
    std::vector<std::vector<int>> twoSum(std::vector<int> &nums, int l, int r, int target) {
        std::vector<std::vector<int>> results;
        // int l = 0; 
        // int r = nums.size() - 1;
        while (l < r) {
            int sum = nums[l] + nums[r];
            if (sum == target) {
                results.push_back({nums[l], nums[r]});
                
                ++l;
                while (l < r && nums[l] == nums[l-1]) {
                    ++l;
                }

                --r;
                while (l < r && nums[r] == nums[r+1]) {
                    --r;
                }
            } else if (sum < target) {
                ++l;
            } else {
                --r;
            }
        }
        return results;
    }

    std::vector<std::vector<int>> kSum(vector<int> &nums, int l, int r, int target, int k) {
        std::vector<std::vector<int>> results;
        for (int i = l; i <= r; ++i) {
            if (i > l && nums[i] == nums[i-1]) {
                continue;
            }
            int next_target = target - nums[i];
            // 迭代
            std::vector<std::vector<int>> tmp_results;
            if (k - 1 != 2) {
                tmp_results = kSum(nums, i + 1, nums.size() - 1, next_target, k -1);
            } else {
                tmp_results = twoSum(nums, i + 1, nums.size() - 1, next_target);
            }
            
            for (int j = 0; j < tmp_results.size(); ++j) {
                std::vector<int> tmp_result = tmp_results[j];
                tmp_result.insert(tmp_result.begin(), nums[i]);
                results.push_back(tmp_result);
            }
        }
        return results;
    }

    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        // 只排序一次 防止递归内部重复排序
        std::sort(nums.begin(), nums.end());
        return kSum(nums, 0, nums.size() - 1, target, 4);
    }
};
// @lc code=end

