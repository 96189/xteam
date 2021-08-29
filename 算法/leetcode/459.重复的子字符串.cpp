/*
 * @lc app=leetcode.cn id=459 lang=cpp
 *
 * [459] 重复的子字符串
 */

// @lc code=start
class Solution {
public:
    bool repeatedSubstringPattern(string s) {
        size_t len = s.length();
        size_t sub_s_len = len / 2;
        while (sub_s_len > 0) {
            if (len % sub_s_len == 0) {
                string news;
                string subs = s.substr(0, sub_s_len);
                size_t group_cnt = len / sub_s_len;
                for (size_t i = 0; i < group_cnt; ++i) {
                    news += subs;
                }
                if (news == s) {
                    return true;
                }
            }
            --sub_s_len;
        }
        return false;
    }
};
// @lc code=end

