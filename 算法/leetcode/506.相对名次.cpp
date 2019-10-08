/*
 * @lc app=leetcode.cn id=506 lang=cpp
 *
 * [506] 相对名次
 *
 * https://leetcode-cn.com/problems/relative-ranks/description/
 *
 * algorithms
 * Easy (47.81%)
 * Total Accepted:    2.4K
 * Total Submissions: 5K
 * Testcase Example:  '[5,4,3,2,1]'
 *
 * 给出 N 名运动员的成绩，找出他们的相对名次并授予前三名对应的奖牌。前三名运动员将会被分别授予 “金牌”，“银牌” 和“ 铜牌”（"Gold
 * Medal", "Silver Medal", "Bronze Medal"）。
 * 
 * (注：分数越高的选手，排名越靠前。)
 * 
 * 示例 1:
 * 
 * 
 * 输入: [5, 4, 3, 2, 1]
 * 输出: ["Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"]
 * 解释: 前三名运动员的成绩为前三高的，因此将会分别被授予 “金牌”，“银牌”和“铜牌” ("Gold Medal", "Silver Medal"
 * and "Bronze Medal").
 * 余下的两名运动员，我们只需要通过他们的成绩计算将其相对名次即可。
 * 
 * 提示:
 * 
 * 
 * N 是一个正整数并且不会超过 10000。
 * 所有运动员的成绩都不相同。
 * 
 * 
 */
class Solution {
public:
    vector<string> findRelativeRanks(vector<int>& nums) {
        vector<int> rank;   // 值排名在原数组中对应的索引 
        for (size_t i = 0; i < nums.size(); ++i)
        {
            rank.push_back(i);
        }
        sort(rank.begin(), rank.end(), [&](const int a, const int b) -> bool {return nums[a] > nums[b];});

        // index为索引 value为对应的排名字符串
        vector<string> ranks(nums.size());
        for (size_t j = 3; j < nums.size(); ++j)
        {
            ranks[rank[j]] = to_string(j + 1);
        }

        if (nums.size() >= 1)
            ranks[rank[0]] = string("Gold Medal");
        if (nums.size() >= 2)
            ranks[rank[1]] = string("Silver Medal");
        if (nums.size() >= 3)
            ranks[rank[2]] = string("Bronze Medal");

        return ranks;
    }
};

