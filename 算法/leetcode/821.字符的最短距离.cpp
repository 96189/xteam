/*
 * @lc app=leetcode.cn id=821 lang=cpp
 *
 * [821] 字符的最短距离
 */
class Solution {
public:
    int minDist(int curIdx, vector<int> &indexs)
    {
        int min = INT_MAX;
        for (size_t i = 0; i < indexs.size(); ++i)
        {
            int dist = abs(indexs[i] - curIdx);
            if (dist < min)
                min = dist;
        }
        return min;
    }
    vector<int> shortestToChar(string S, char C)
    {
        // 记录c在s中的所有位置索引
        vector<int> indexs;
        for (size_t i = 0; i < S.size(); ++i)
        {
            if (S[i] == C)
            {
                indexs.push_back(i);
            }
        }
        vector<int> dists;
        if (indexs.empty())
            return dists;
        // 遍历字符串 计算距离值
        for (size_t i = 0; i < S.size(); ++i)
        {
            if (S[i] == C)
                dists.push_back(0);
            else
                dists.push_back(minDist(i, indexs));
        }
        return dists;
    }
};

