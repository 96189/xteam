/*
 * @lc app=leetcode.cn id=884 lang=cpp
 *
 * [884] 两句话中的不常见单词
 */
class Solution {
public:
    vector<string> uncommonFromSentences(string A, string B) {
        vector<string> uncommon;
        unordered_map<string, int> stringCnt;
        stringstream ssA(A);
        stringstream ssB(B);
        string word;
        while (ssA >> word)
        {
            if (!word.empty())
                ++stringCnt[word];
        }
        while (ssB >> word)
        {
            if (!word.empty())
                ++stringCnt[word];
        }
        for (auto &kvItem : stringCnt)
        {
            if (kvItem.second == 1)
            {
                uncommon.push_back(kvItem.first);
            }
        }
        return uncommon;
    }
};

