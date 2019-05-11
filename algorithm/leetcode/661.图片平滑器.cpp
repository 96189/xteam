/*
 * @lc app=leetcode.cn id=661 lang=cpp
 *
 * [661] 图片平滑器
 */
class Solution {
public:
    vector<vector<int>> imageSmoother(vector<vector<int>>& M) {
        vector<vector<int>> grayscale;
        if (M.empty() || M[0].empty())
            return grayscale;

        int rowCnt = M.size();
        int colCnt = M[0].size();
        for (int row = 0; row < rowCnt; ++row)
        {
            vector<int> scale;
            for (int col = 0; col < colCnt; ++col)
            {
                int sum = 0;
                int cnt = 0;
                for (int i = max(0, row-1); i <= min(rowCnt-1, row+1); ++i)
                {
                    for (int j = max(0, col-1); j <= min(colCnt-1, col+1); ++j)
                    {
                        sum += M[i][j];
                        ++cnt;
                    }
                }
                scale.push_back(sum / cnt);
            }
            grayscale.push_back(scale);
        }
        return grayscale;
    }
};

