/*
 * @lc app=leetcode.cn id=541 lang=cpp
 *
 * [541] 反转字符串 II
 */
class Solution {
public:
    string reverseStr(string s, int k) {
        if (s.empty())
            return s;
        int len = s.size() - 1;
        int dist = 2 * k;
        int groupstart = 0;
        while (groupstart + dist < len)
        {
            reverse(s.begin() + groupstart, s.begin() + groupstart + k);
            groupstart = groupstart + dist;
        }
        if (len - groupstart >= k)
            reverse(s.begin() + groupstart, s.begin() + groupstart + k);
        else
            reverse(s.begin() + groupstart, s.end());
        return s;
    }
};

