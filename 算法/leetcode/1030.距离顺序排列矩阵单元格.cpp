/*
 * @lc app=leetcode.cn id=1030 lang=cpp
 *
 * [1030] 距离顺序排列矩阵单元格
 */
class Solution {
public:
    vector<vector<int>> allCellsDistOrder(int R, int C, int r0, int c0) {
        vector<vector<int>> pointSeq;
        for (int r = 0; r < R; ++r)
        {
            for (int c = 0; c < C; ++c)
            {
                vector<int> point;
                point.push_back(r);
                point.push_back(c);
                pointSeq.push_back(point);
            }
        }
        sort(pointSeq.begin(), pointSeq.end(),
             [&](const vector<int> &a, const vector<int> &b) -> bool {
                 return abs(a[0] - r0) + abs(a[1] - c0) < abs(b[0] - r0) + abs(b[1] - c0);
             });
        return pointSeq;
    }
};

