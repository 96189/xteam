/*
 * @lc app=leetcode.cn id=665 lang=cpp
 *
 * [665] 非递减数列
 */
class Solution {
public:
    bool checkPossibility(vector<int>& nums) {
        int cnt = 0;
        for (int i = 1; i < nums.size() && cnt <= 1; ++i)
        {
            if (nums[i - 1] > nums[i])
            {
                ++cnt;
                // 判断失序的两个值应该修改哪一个值
                if (i - 2 < 0 || nums[i - 2] <= nums[i])
                {
                    nums[i - 1] = nums[i];
                }
                else
                {
                    nums[i] = nums[i - 1];
                }
            }
        }
        return cnt <= 1;
    }
};

