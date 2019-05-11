/*
 * @lc app=leetcode.cn id=709 lang=cpp
 *
 * [709] To Lower Case
 */
class Solution {
public:
    string toLowerCase(string str) {
        if (str.empty())
            return str;
        for (auto & ch : str)
        {
            if (ch >= 'A' && ch <= 'Z')
                ch += 'a' - 'A';
        }
        return str;
    }
};

