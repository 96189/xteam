/*
 * @lc app=leetcode.cn id=38 lang=cpp
 *
 * [38] 报数
 */
class Solution {
public:
    string countAndSay(string s, int depth, int n)
    {
        if (depth == n)
            return s;
        string ans;
        int cnt = 1; // 默认填充1
        for (size_t i = 0; i < s.length(); ++i)
        {
            if (i + 1 < s.length() && s[i] == s[i + 1])
            {
                ++cnt;
            }
            else
            {
                ans += cnt + '0';
                ans += s[i];
                cnt = 1; // 默认填充1
            }
        }
        return countAndSay(ans, depth + 1, n);
    }
    string countAndSay(int n) {
        return countAndSay("1", 1, n);
    }
};

