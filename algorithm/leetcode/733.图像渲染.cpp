/*
 * @lc app=leetcode.cn id=733 lang=cpp
 *
 * [733] 图像渲染
 */
class Solution {
public:
    // void udlrPoints(int sr, int sc, queue<pair<int, int>> &pointQ, vector<vector<int>> &image, int oldColor)
    // {
    //     int maxRow = image.size() - 1;
    //     int maxCol = image[0].size() - 1;
    //     int mR = min(maxRow, sr + 1);
    //     int mC = min(maxCol, sc + 1);
    //     for (int row = sr - 1; row <= mR; ++row)
    //     {
    //         for (int col = sc - 1; col <= mC; ++col)
    //         {
    //             if (row < 0 || col < 0)
    //                 continue;
    //             if (row == sr && col == sc)
    //                 continue;
    //             if (row == sr || col == sc)
    //             {
    //                 if (image[row][col] == oldColor)
    //                 {
    //                     pointQ.push(make_pair(row, col));
    //                 }
    //             }
    //         }
    //     }
    // }
    // vector<vector<int>> floodFill(vector<vector<int>> &image, int sr, int sc, int newColor)
    // {
    //     int oldColor = image[sr][sc];
    //     if (oldColor == newColor)
    //         return image;
    //     queue<pair<int, int>> pointQ;
    //     image[sr][sc] = newColor;
    //     udlrPoints(sr, sc, pointQ, image, oldColor);

    //     while (!pointQ.empty())
    //     {
    //         auto &val = pointQ.front();
    //         image[val.first][val.second] = newColor;
    //         udlrPoints(val.first, val.second, pointQ, image, oldColor);
    //         pointQ.pop();
    //     }
    //     return image;
    // }
    vector<vector<int>> floodFill(vector<vector<int>> &image, int sr, int sc, int newColor)
    {
        int oldColor = image[sr][sc];
        if (oldColor != newColor)
        {
            dfs(image, sr, sc, oldColor, newColor);
        }
        return image;
    }
    void dfs(vector<vector<int>> &image, int row, int col, int oldColor, int newColor)
    {
        int maxRow = image.size();
        int maxCol = image[0].size();
        if (row < 0 || row >= maxRow || col < 0 || col >= maxCol ||
            image[row][col] != oldColor)
        {
            return;
        }
        image[row][col] = newColor;
        dfs(image, row-1, col, oldColor, newColor);
        dfs(image, row+1, col, oldColor, newColor);
        dfs(image, row, col-1, oldColor, newColor);
        dfs(image, row, col+1, oldColor, newColor);
    }
};

