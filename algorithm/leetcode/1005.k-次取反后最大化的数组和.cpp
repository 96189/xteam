/*
 * @lc app=leetcode.cn id=1005 lang=cpp
 *
 * [1005] K 次取反后最大化的数组和
 */
class Solution {
public:
    int largestSumAfterKNegations(vector<int>& A, int K) {
        // 假如存在负数  则优先取反最小的负数 
        sort(A.begin(), A.end());
        for (auto &v : A)
        {
            if (v < 0 && K > 0)
            {
                v = -v;
                K--;
            }
        }
        sort(A.begin(), A.end());
        // 剩余K次取反 则K次都作用于第一个数据
        // K为奇数 则将最小的数字取反
        // K为偶数 则最小的数字取反偶数次不变
        if (K % 2 == 1)
        {
            A[0] = -A[0];
        }
        int ans = 0;
        for (auto v : A)
        {
            ans += v;
        }
        return ans;
    }
};

