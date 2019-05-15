/*
 * @lc app=leetcode.cn id=868 lang=cpp
 *
 * [868] 二进制间距
 */
class Solution {
public:
    int binaryGap(int N) {
        int dist = 0, pre = -1;
        for (int i = 0; i < 32; i++)
        {
            if ((N & (1 << i)) != 0)
            {
                if (pre != -1)
                    dist = max(dist, i - pre);
                pre = i;
            }
        }
        return dist;
    }
};

