/*
 * @lc app=leetcode.cn id=1002 lang=cpp
 *
 * [1002] 查找常用字符
 */
class Solution {
public:
    vector<string> commonChars(vector<string>& A) {
        vector<string> common;
        if (A.empty())
            return common;
        int max = INT_MAX;
        int alphaCnt = 26;
        vector<int> alphabet(alphaCnt, max);
        for (auto &s : A)
        {
            // hash表统计字符次数
            vector<int> count(alphaCnt, 0);
            for (auto &ch : s)
            {
                ++count[ch - 'a'];
            }
            // 每个字符出现的最小次数
            for (int i = 0; i < alphaCnt; ++i)
            {
                alphabet[i] = min(alphabet[i], count[i]);
            }
        }
        for (int i = 0; i < alphaCnt; ++i)
        {
            if (alphabet[i] != max && alphabet[i] != 0)
            {
                string str;
                str += ('a' + i);
                for (int j = 0; j < alphabet[i]; ++j)
                    common.push_back(str);
            }
        }
        return common;
    }
};

