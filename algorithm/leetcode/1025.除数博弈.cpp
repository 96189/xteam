/*
 * @lc app=leetcode.cn id=1025 lang=cpp
 *
 * [1025] 除数博弈
 */
class Solution {
public:
    bool divisorGame(int N) {
        // 2 win
        // 3 lose
        // 4 win
        // 5 lose
        // 6 win
        // 7 lose
        // 8 win
        // alice拿到的数字N为偶数 且alice第一步选择1 则必赢
        return N % 2 == 0;
    }
};

