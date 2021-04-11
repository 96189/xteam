/*
 * @lc app=leetcode.cn id=811 lang=cpp
 *
 * [811] 子域名访问计数
 */

// @lc code=start
class Solution {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        vector<string> result;
        map<string, int> domain2cnt;
        for (auto & item : cpdomains) {
            size_t index = item.find(" ");
            int count = stoi(item.substr(0, index));

            while (index != string::npos) {
                domain2cnt[item.substr(index + 1)] += count;
                index = item.find(".", index + 1);
            }
        }
        for (auto & info : domain2cnt) {
            result.push_back(to_string(info.second) + " " + info.first);
        }
        return result;
    }
};
// @lc code=end

