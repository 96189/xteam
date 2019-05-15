/*
 * @lc app=leetcode.cn id=830 lang=cpp
 *
 * [830] 较大分组的位置
 */
class Solution {
public:
    vector<vector<int>> largeGroupPositions(string S) {
        vector<vector<int>> startEndVec;
        size_t start = 0;
        size_t next = start + 1;
        // 解决"aaa"这种情况 循环无法处理的问题
        S.push_back('@');
        while (next < S.size())
        {
            if (S[next] == S[start])
            {
                ++next;
            }
            else
            {
                int diff = next - start;
                if (diff >= 3)
                {
                    vector<int> pointPair;
                    pointPair.push_back(start);
                    pointPair.push_back(next - 1);
                    startEndVec.push_back(pointPair);
                }
                start = next;
                next = start + 1;
            }
        }
        return startEndVec;
    }
};

