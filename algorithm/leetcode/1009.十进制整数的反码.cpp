/*
 * @lc app=leetcode.cn id=1009 lang=cpp
 *
 * [1009] 十进制整数的反码
 */
class Solution {
public:
    int bitwiseComplement(int N) {
        if (N == 0)
            return 1;
        int ans = 0;
        int i = 0;
        while (N)
        {
            if ((N & 1) == 0)
            {
                ans += pow(2, i);
            }
            N >>= 1;
            ++i;
        }
        return ans;
    }
};

