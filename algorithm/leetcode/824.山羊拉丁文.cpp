/*
 * @lc app=leetcode.cn id=824 lang=cpp
 *
 * [824] 山羊拉丁文
 */
class Solution {
public:
    string toGoatLatin(string S) {
        unordered_set<char> vowel = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
        // 预处理 字符串按空格分割 转小写
        string newS;
        stringstream ss(S);
        string word;
        string suffix = "ma";
        int index = 0;
        while (ss >> word)
        {
            if (word.empty())
                continue;
            ++index;
            // 首字母为元音
            if (vowel.count(word[0]) != 0)
            {
                word += suffix;
                int count = index;
                while (count-- > 0)
                    word.push_back('a');
                newS += word + " ";
            }
            // 辅音
            else
            {
                char first = word[0];
                for (size_t i = 1; i < word.length(); ++i)
                {
                    word[i - 1] = word[i];
                }
                word[word.length() - 1] = first;
                word += suffix;
                int count = index;
                while (count-- > 0)
                    word.push_back('a');
                newS += word + " ";
            }
        }
        newS.pop_back();
        return newS;
    }
};

