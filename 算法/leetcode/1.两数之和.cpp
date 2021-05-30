/*
 * @lc app=leetcode.cn id=1 lang=cpp
 *
 * [1] 两数之和
 */

// @lc code=start
class Solution {
public:
    // 数组有序
    // vector<int> twoSum(vector<int>& nums, int target) {
    //     vector<int> result;
    //     std::sort(nums.begin(), nums.end());
    //     int s = 0;
    //     int e = nums.size() - 1;
    //     while (e > s) {
    //         int sum = nums[s] + nums[e];
    //         if (sum == target) {
    //             result.push_back(s);
    //             result.push_back(e);
    //             break;
    //         } else if (sum < target) {
    //             ++s;
    //         } else {
    //             --e;
    //         }
    //     }
    //     return result;
    // }

    // 数组无序
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, int> hash;
        for (int i = 0; i < nums.size(); ++i) {
            if (hash.find(target - nums[i]) != hash.end()) {
                return vector<int>{hash[target - nums[i]], i};
            }
            hash[nums[i]] = i;
        }
        return vector<int>{-1, -1};
    }
};
// @lc code=end

