/*
 * @lc app=leetcode.cn id=122 lang=cpp
 *
 * [122] 买卖股票的最佳时机 II
 */
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int ans = 0;
        int n = prices.size();
        for (int i = 1; i < n; ++i)
        {
            int profit = prices[i] - prices[i - 1];
            if (profit > 0)
            {
                ans += profit;
            }
        }
        return ans;
    }
};

