/*
 * @lc app=leetcode.cn id=796 lang=cpp
 *
 * [796] 到达终点
 */
class Solution {
public:
    bool rotateString(string A, string B) {
        if (A.empty() && B.empty())
            return true;
        if (A.length() != B.length())
            return false;

        int len = A.length();
        for (int i = 0; i < len; i++)
            if (A.substr(i, len) + A.substr(0, i) == B)
                return true;
        return false;
    }
};

