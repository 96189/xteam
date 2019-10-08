/*
 * @lc app=leetcode.cn id=804 lang=cpp
 *
 * [804] 旋转数字
 */
class Solution {
public:
    int uniqueMorseRepresentations(vector<string>& words) {
        vector<string> strhash = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."};
        map<string, int> convertstr2Cnt;
        for (auto &str : words)
        {
            string convert = "";
            for (auto &ch : str)
                convert += strhash[ch - 'a'];
            ++convertstr2Cnt[convert];
        }
        return convertstr2Cnt.size();
    }
};

