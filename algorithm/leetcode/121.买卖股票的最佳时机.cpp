/*
 * @lc app=leetcode.cn id=121 lang=cpp
 *
 * [121] 买卖股票的最佳时机
 */
class Solution {
public:
    int maxProfit(vector<int> &prices)
    {
		int ans = 0;
		int buy = INT_MAX;
		for (int price : prices) 
        {
			buy = min(buy, price);
			ans = max(ans, price - buy);
		}
		return ans;
    }
};

