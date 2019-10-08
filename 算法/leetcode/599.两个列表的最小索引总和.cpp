/*
 * @lc app=leetcode.cn id=599 lang=cpp
 *
 * [599] 两个列表的最小索引总和
 */
class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        vector<string> same;
        if (list1.empty() || list2.empty())
            return same;
        vector<pair<int, string>> vec;
        // 建表
        unordered_map<string, int> hash;
        for (size_t i = 0; i < list1.size(); ++i)
        {
            hash[list1[i]] = i + 1;
        }
        // 查表
        for (size_t j = 0; j < list2.size(); ++j)
        {
            if (hash[list2[j]] != 0)
                vec.push_back(make_pair(j + hash[list2[j]], list2[j]));
            // same.push_back(val);
        }
        // 排序
        sort(vec.begin(), vec.end(), [](const pair<int, string> &a, const pair<int, string> &b) { return a.first < b.first; });
        same.push_back(vec[0].second);
        return same;
    }
};

