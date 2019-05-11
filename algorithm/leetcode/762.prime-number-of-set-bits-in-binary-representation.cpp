/*
 * @lc app=leetcode.cn id=762 lang=cpp
 *
 * [762] Prime Number of Set Bits in Binary Representation
 */
class Solution {
public:
    int countPrimeSetBits(int L, int R) {
        int cnt = 0;
        // 1000000的二进制有19位
        vector<int> primes = { 2, 3, 5, 7, 11, 13, 17, 19  };
        while (L <= R)
        {
            int bit1Cnt = 0;
            int val = L;
            while (val)
            {
                val = val & (val - 1);
                ++bit1Cnt;
            }
            if (find(primes.begin(), primes.end(), bit1Cnt) != primes.end())
            {
                ++cnt;
            }
            ++L;
        }
        return cnt;
    }
};

