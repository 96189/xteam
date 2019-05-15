/*
 * @lc app=leetcode.cn id=860 lang=cpp
 *
 * [860] 柠檬水找零
 */
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        if (bills.empty())
            return true;
        unordered_map<int, int> changeCnt;
        for (auto &money : bills)
        {
            ++changeCnt[money];
            if (money != 5)
            {
                money -= 5;
                while (money >= 20 && changeCnt[20] > 0)
                {
                    money -= 20;
                    --changeCnt[20];
                }
                while (money >= 10 && changeCnt[10] > 0)
                {
                    money -= 10;
                    --changeCnt[10];
                }
                while (money >= 5 && changeCnt[5] > 0)
                {
                    money -= 5;
                    --changeCnt[5];
                }
                if (money > 0)
                    return false;
            }
        }
        return true;
    }
};

