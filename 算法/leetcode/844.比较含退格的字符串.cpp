/*
 * @lc app=leetcode.cn id=844 lang=cpp
 *
 * [844] 比较含退格的字符串
 */
class Solution {
public:
    bool backspaceCompare(string S, string T) {
        vector<char> vs;
        vector<char> vt;
        for (auto &ch : S)
        {
            if (ch == '#')
            {
                if (!vs.empty())
                    vs.pop_back();
            }
            else
            {
                vs.push_back(ch);
            }
        }
        for (auto &ch : T)
        {
            if (ch == '#')
            {
                if (!vt.empty())
                    vt.pop_back();
            }
            else
            {
                vt.push_back(ch);
            }
        }
        return vs == vt;
    }
};

