/*
 * @lc app=leetcode.cn id=557 lang=cpp
 *
 * [557] 反转字符串中的单词 III
 */
class Solution {
public:
    string reverseWords(string s) {
        if (s.empty())
            return s;
        size_t i = 0;
        // 预处理 - 跳过最前面的空格
        while (i < s.size())
        {
            if (!isspace(s[i]))
                break;
            ++i;
        }
        if (i >= s.size())
            return s;
        // 处理
        size_t j = i;
        while (j < s.size())
        {
            if (isspace(s[j]))
            {
                reverse(s.begin() + i, s.begin() + j);
                i = j;
                while (i < s.size())
                {
                    if (!isspace(s[i]))
                        break;
                    ++i;
                }
                j = i;
            }
            else
            {
                ++j;
            }
        }
        reverse(s.begin() + i, s.begin() + j);
        return s;
    }
};

