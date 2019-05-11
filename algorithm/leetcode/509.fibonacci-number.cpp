/*
 * @lc app=leetcode.cn id=509 lang=cpp
 *
 * [509] Fibonacci Number
 */
class Solution {
public:
    int fib(int N) {
        vector<int> fibseqs(2);
        fibseqs[0] = 0;
        fibseqs[1] = 1;
        for (int i = 2; i <= N; ++i)
        {
            fibseqs.push_back(fibseqs[i-1] + fibseqs[i-2]);
        }
        return fibseqs[N];
    }
};

