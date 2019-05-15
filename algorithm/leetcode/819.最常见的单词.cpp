/*
 * @lc app=leetcode.cn id=819 lang=cpp
 *
 * [819] 最常见的单词
 */
class Solution {
public:
    string mostCommonWord(string paragraph, vector<string>& banned) {
        unordered_set<string> bannedWords;
        for (auto &bw : banned)
        {
            bannedWords.insert(bw);
        }
        //
        unordered_map<string, int> str2Cnt;
        // 预处理 按照空格分割 去掉标点 转小写 排除过滤值统计
        string word;
        // 若字符串已字母结尾 则在最后添加. 保证循环能处理最后一个单词
        if (isalpha(paragraph[paragraph.size()-1]))
        {
            paragraph.push_back('.');
        }
        for (size_t i = 0; i < paragraph.size(); ++i)
        {
            if (isalpha(paragraph[i]))
            {
                word.push_back(paragraph[i]);
            }
            else
            {
                if (!word.empty())
                {
                    transform(word.begin(), word.end(), word.begin(), ::tolower);
                    if (bannedWords.count(word) == 0)
                    {
                        ++str2Cnt[word];
                    }
                }
                word.clear();
            }
        }
        // 找最大
        int maxCnt = 0;
        string maxStr = "";
        for (auto &kvItem : str2Cnt)
        {
            if (kvItem.second > maxCnt)
            {
                maxCnt = kvItem.second;
                maxStr = kvItem.first;
            }
        }
        return maxStr;
    }
};

