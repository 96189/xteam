/*
 * @lc app=leetcode.cn id=504 lang=cpp
 *
 * [504] 七进制数
 *
 * https://leetcode-cn.com/problems/base-7/description/
 *
 * algorithms
 * Easy (43.31%)
 * Total Accepted:    3.3K
 * Total Submissions: 7.6K
 * Testcase Example:  '100'
 *
 * 给定一个整数，将其转化为7进制，并以字符串形式输出。
 * 
 * 示例 1:
 * 
 * 
 * 输入: 100
 * 输出: "202"
 * 
 * 
 * 示例 2:
 * 
 * 
 * 输入: -7
 * 输出: "-10"
 * 
 * 
 * 注意: 输入范围是 [-1e7, 1e7] 。
 * 
 */
class Solution {
public:
    string convertToBase7(int num) {
        if (num == 0) return "0";
        bool flag = false;
        if (num < 0)
        {
            num *= -1;
            flag = true;
        }
        string s = "";
        while (num != 0)
        {
            s += num % 7 + '0';
            num = num / 7;
        }
        reverse(s.begin(), s.end());
        if (flag)
            s = '-' + s;
        return s;
    }
};

