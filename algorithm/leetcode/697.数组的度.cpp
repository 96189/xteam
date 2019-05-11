/*
 * @lc app=leetcode.cn id=697 lang=cpp
 *
 * [697] 数组的度
 */
class Solution {
public:
    int findShortestSubArray(vector<int>& nums) {
        if (nums.empty())
            return 0;
        // 建表 key:num value:index
        map<int, vector<int>> num2indexs;
        for (size_t i = 0; i < nums.size(); ++i)
        {
            num2indexs[nums[i]].push_back(i);
        }
        // key:num value:count
        vector<pair<int, int>> numCount;
        for (auto &item : num2indexs)
        {
            numCount.push_back(make_pair(item.first, item.second.size()));
        }
        // 排序
        sort(numCount.begin(), numCount.end(), [](const pair<int, int> &a, const pair<int, int> &b) -> bool { return a.second > b.second; });
        // count最大的保存
        vector<int> maxCount;
        int cnt = numCount[0].second;
        maxCount.push_back(numCount[0].first);
        for (size_t i = 1; i < numCount.size(); ++i)
        {
            if (cnt == numCount[i].second)
            {
                maxCount.push_back(numCount[i].first);
            }
        }
        // 计算索引差
        vector<int> dist;
        for (size_t i = 0; i < maxCount.size(); ++i)
        {
            auto &&vec = num2indexs[maxCount[i]];
            dist.push_back(vec[vec.size() - 1] - vec[0] + 1);
        }
        // 索引差排序取最小
        sort(dist.begin(), dist.end());
        return dist[0];
    }
};

