/*
 * @lc app=leetcode.cn id=989 lang=cpp
 *
 * [989] 数组形式的整数加法
 */
class Solution {
public:
    vector<int> addToArrayForm(vector<int>& A, int K) {
        // 转换K为数组形式
        vector<int> KA;
        while (K)
        {
            KA.push_back(K % 10);
            K /= 10;
        }
        int lenA = A.size();
        int lenKA = KA.size();
        int difflen = abs(lenA - lenKA);
        reverse(A.begin(), A.end());
        while (difflen-- > 0)
        {
            if (lenKA > lenA)
            {
                A.push_back(0);
            }
            else
            {
                KA.push_back(0);
            }
        }
        // 数组模式相加
        vector<int> newA;
        int sa = 0;
        int ska = 0;
        int carry = 0;
        while (sa < lenA && ska < lenKA)
        {
            int sum = carry + A[sa] + KA[ska];
            if (sum >= 10)
                carry = 1;
            else
                carry = 0;
            newA.push_back(sum % 10);
            ++sa;
            ++ska;
        }
        while (sa < lenA)
        {
            int sum = carry + A[sa];
            if (sum >= 10)
                carry = 1;
            else
                carry = 0;
            newA.push_back(sum % 10);
            ++sa;
        }
        while (ska < lenKA)
        {
            int sum = carry + KA[ska];
            if (sum >= 10)
                carry = 1;
            else
                carry = 0;
            newA.push_back(sum % 10);
            ++ska;
        }
        if (carry)
        {
            newA.push_back(carry);
        }
        reverse(newA.begin(), newA.end());
        return newA;
    }
};

