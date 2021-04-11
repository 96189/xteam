/*
 * @lc app=leetcode.cn id=599 lang=cpp
 *
 * [599] 两个列表的最小索引总和
 */

// @lc code=start
class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        map<string, int> name2index;
        for (size_t i = 0; i < list1.size(); ++i) {
            name2index[list1[i]] = i;
        }
        map<int, vector<string>> result;
        for (size_t i = 0; i < list2.size(); ++i) {
            auto it = name2index.find(list2[i]);
            if (it == name2index.end()) {
                continue;
            }
            size_t sum = i + it->second;
            result[sum].push_back(list2[i]);
        }
        size_t sum = result.begin()->first;
        for (auto it = result.begin(); it != result.end(); ++it) {
            if (it->first < sum) {
                sum = it->first;
            }
        }
        auto & vec = result.find(sum)->second;
        return vec;
    }
};
// @lc code=end

