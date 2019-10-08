/*
 * @lc app=leetcode.cn id=704 lang=cpp
 *
 * [704] Binary Search
 */
class Solution {
public:
    int search(vector<int>& nums, int target) {
        if (nums.empty())
            return -1;
        int s = 0;
        int e = nums.size() - 1;
        while (s <= e)
        {
            int mid = e - (e - s) / 2;
            if (nums[mid] == target)
                return mid;
            else if (nums[mid] > target)
                e = mid - 1;
            else
                s = mid + 1;
        }
        return -1;
    }
};

