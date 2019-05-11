/*
 * @lc app=leetcode.cn id=476 lang=cpp
 *
 * [476] 数字的补数
 *
 * https://leetcode-cn.com/problems/number-complement/description/
 *
 * algorithms
 * Easy (67.09%)
 * Total Accepted:    8.6K
 * Total Submissions: 12.8K
 * Testcase Example:  '5'
 *
 * 给定一个正整数，输出它的补数。补数是对该数的二进制表示取反。
 * 
 * 注意:
 * 
 * 
 * 给定的整数保证在32位带符号整数的范围内。
 * 你可以假定二进制数不包含前导零位。
 * 
 * 
 * 示例 1:
 * 
 * 
 * 输入: 5
 * 输出: 2
 * 解释: 5的二进制表示为101（没有前导零位），其补数为010。所以你需要输出2。
 * 
 * 
 * 示例 2:
 * 
 * 
 * 输入: 1
 * 输出: 0
 * 解释: 1的二进制表示为1（没有前导零位），其补数为0。所以你需要输出0。
 * 
 * 
 */
class Solution {
public:
    long highestOneBit(long i)
    {
        // HD, Figure 3-1
        i |= (i >> 1);
        i |= (i >> 2);
        i |= (i >> 4);
        i |= (i >> 8);
        i |= (i >> 16);
        i |= (i >> 32);
        return i - (i >> 1);
    }
    int findComplement(int num)
    {
        int val = num;
        long shift = 1;
        while (val > 0)
        {
            shift <<= 1;
            val >>= 1;
        }
        long mask = shift - 1;
        return ~num & mask;
        // // 计算掩码值 
        // int mask = (highestOneBit(num) << 1) - 1;
        // return num ^ mask;
    }
};

