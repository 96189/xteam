/*
 * @lc app=leetcode.cn id=693 lang=cpp
 *
 * [693] 交替位二进制数
 */
class Solution {
public:
    bool hasAlternatingBits(int n) {
        if (n % 2 != 0)
            n = n >> 1;
        if (n % 2 != 0)
            return false;
        int idx = 1;
        while (n > 0)
        {
            n -= pow(2, idx);
            idx += 2;
        }
        return n == 0;
    }
};

