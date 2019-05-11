/*
 * @lc app=leetcode.cn id=771 lang=cpp
 *
 * [771] Jewels and Stones
 */
class Solution {
public:
    int numJewelsInStones(string J, string S) {
        if (J.empty())
            return 0;
        // 建表
        map<char, int> ch2Cnt;
        for (auto &ch : S)
        {
            ++ch2Cnt[ch];
        }
        // 求和
        int sum = 0;
        for (auto &ch : J)
        {
            sum += ch2Cnt[ch];
        }
        return sum;
    }
};

