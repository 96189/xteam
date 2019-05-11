/*
 * @lc app=leetcode.cn id=551 lang=cpp
 *
 * [551] 学生出勤记录 I
 */
class Solution {
public:
    bool checkRecord(string s) {
        int cntA = 0;
        int continueCntL = 0;
        bool isPrevL = false;
        for (auto &ch : s)
        {
            if (!isPrevL)
            {
                continueCntL = 0;
            }

            isPrevL = false;
            if (ch == 'A')
                ++cntA;
            else if (ch == 'L')
            {
                ++continueCntL;
                isPrevL = true;
            }

            if (continueCntL > 2)
                return false;
        }
        return cntA <= 1;;
    }
};

