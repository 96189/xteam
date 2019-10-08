/*
 * @lc app=leetcode.cn id=532 lang=cpp
 *
 * [532] 数组中的K-diff数对
 */
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        if (k < 0)
           return 0;
        // hash表
        int cnt = 0;
        map<long, int> hash;
        for (auto &val : nums)
        {
            // hash[val] = 1;
            ++hash[val];
        }
        // for (auto it = hash.begin(); it != hash.end(); ++it)
        for (auto & item : hash)
        {
            if (k != 0 && hash.find(item.first + k) != hash.end())
                ++cnt;
            if (k == 0 && item.second >= 2/*count(nums.begin(), nums.end(), it->first) >= 2*/)
                ++cnt;
        }
        return cnt;
    }
};

