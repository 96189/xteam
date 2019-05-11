/*
 * @lc app=leetcode.cn id=453 lang=cpp
 *
 * [453] 最小移动次数使数组元素相等
 *
 * https://leetcode-cn.com/problems/minimum-moves-to-equal-array-elements/description/
 *
 * algorithms
 * Easy (49.72%)
 * Total Accepted:    3K
 * Total Submissions: 6.1K
 * Testcase Example:  '[1,2,3]'
 *
 * 给定一个长度为 n 的非空整数数组，找到让数组所有元素相等的最小移动次数。每次移动可以使 n - 1 个元素增加 1。
 * 
 * 示例:
 * 
 * 
 * 输入:
 * [1,2,3]
 * 
 * 输出:
 * 3
 * 
 * 解释:
 * 只需要3次移动（注意每次移动会增加两个元素的值）：
 * 
 * [1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]
 * 
 * 
 */
class Solution {
public:
    int minMoves(vector<int>& nums) {
        // sum + (n-1)*times = n * diff;
        // diff = min + times
        // times = sum - n * min
        if (nums.empty()) return 0;
        // sort(nums.begin(), nums.end());
        // int n = nums.size();
        // int min = nums[0];
        // long long sum = 0;
        // for (auto &val : nums)
        // {
        //     sum += val;
        // }
        // return sum - n * min;

        int min = nums[0];
        for (auto num : nums){
            if (min > num)
                min = num;
        }
        int sum = 0;
        for (size_t i = 0; i < nums.size(); i++)
            sum += nums[i] - min;

        return sum;
    }
};

