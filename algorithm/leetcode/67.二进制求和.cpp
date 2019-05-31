/*
 * @lc app=leetcode.cn id=67 lang=cpp
 *
 * [67] 二进制求和
 */
class Solution {
public:
    string addBinary(string a, string b) {
        string sum = "";
        int carry = 0;
        int i = a.size() - 1;
        int j = b.size() - 1;
        while (i >= 0 && j >= 0)
        {
            int val = a[i] - '0' + b[j] - '0' + carry;
            carry = 0;
            if (val > 1)
            {
                carry = 1;
                val -= 2;
            }
            sum += to_string(val);
            --i;
            --j;
        }
        while (i >= 0)
        {
            int val = a[i] - '0' + carry;
            carry = 0;
            if (val > 1)
            {
                carry = 1;
                val -= 2;
            }
            sum += to_string(val);
            --i;
        }
        while (j >= 0)
        {
            int val = b[j] - '0' + carry;
            carry = 0;
            if (val > 1)
            {
                carry = 1;
                val -= 2;
            }
            sum += to_string(val);
            --j;
        }
        if (carry)
        {
            sum += to_string(carry);
        }
        reverse(sum.begin(), sum.end());
        return sum;
    }
};

