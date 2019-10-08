/*
 * @lc app=leetcode.cn id=977 lang=cpp
 *
 * [977] 有序数组的平方
 */
class Solution {
public:
    vector<int> sortedSquares(vector<int>& A) {
        sort(A.begin(), A.end(), [](const int a, const int b) -> bool {return abs(a) < abs(b);});
        transform(A.begin(), A.end(), A.begin(), [](int &a) -> int { return a * a;});
        return A;
        // if (A.empty())
        //     return A;
        // vector<int> newA;

        // int sizeA = A.size();
        // int i;
        // for (i = 0; i < sizeA; ++i)
        // {
        //     if (A[i] >= 0)
        //         break;
        // }
        // int left = i - 1;
        // int right = i;
        // while (left >= 0 && right < sizeA)
        // {
        //     if (abs(A[left]) <= abs(A[right]))
        //     {
        //         newA.push_back(A[left] * A[left]);
        //         --left;
        //     }
        //     else
        //     {
        //         newA.push_back(A[right] * A[right]);
        //         ++right;
        //     }
        // }
        // while (left >= 0)
        // {
        //     newA.push_back(A[left] * A[left]);
        //     --left;
        // }
        // while (right < sizeA)
        // {
        //     newA.push_back(A[right] * A[right]);
        //     ++right;
        // }
        // return newA;
    }
};

