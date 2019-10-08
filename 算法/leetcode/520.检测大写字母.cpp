/*
 * @lc app=leetcode.cn id=520 lang=cpp
 *
 * [520] 检测大写字母
 */
class Solution {
public:
    bool detectCapitalUse(string word) {
        int wordSize = word.size();
        int upperCnt = 0;
        for (auto &ch : word)
        {
            if (isupper(ch))
            {
                ++upperCnt;
            }
        }
        if (wordSize == upperCnt ||
            upperCnt == 0 ||
            (wordSize > 1 && upperCnt == 1 && isupper(word[0])))
            return true;
        return false;
    }
};

