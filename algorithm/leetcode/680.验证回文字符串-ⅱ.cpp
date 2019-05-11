/*
 * @lc app=leetcode.cn id=680 lang=cpp
 *
 * [680] 验证回文字符串 Ⅱ
 */
class Solution {
public:
    bool validPalindrome(string s) {
        if (s.empty())
            return true;

        int start = 0;
        int end = s.size() - 1;
        int cnt = 0;
        while (start <= end)
        {
            if (s[start] != s[end])
            {
                ++cnt;
            }
            ++start;
            --end;
        }
        if (s.size() == 3)
            return cnt == 0;
        return cnt <= 1;
    }
};

