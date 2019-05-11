/*
 * @lc app=leetcode.cn id=575 lang=cpp
 *
 * [575] 分糖果
 */
class Solution {
public:
    int distributeCandies(vector<int>& candies) {
        if (candies.empty())
            return 0;

        int kindCnt = 0;
        int halfCnt = candies.size() / 2;
        map<int, int> hashvalcnt;
        for (auto &val : candies)
        {
            ++hashvalcnt[val];
        }
        // for (auto &item : hashvalcnt)
        // {
        //     if (kindCnt == halfCnt)
        //         break;
        //     if (item.second == 1)
        //         ++kindCnt;
        // }
        // if (kindCnt != halfCnt)
        // {
        //     for (auto &item : hashvalcnt)
        //     {
        //         if (kindCnt == halfCnt)
        //             break;
        //         if (item.second > 1)
        //         {
        //             ++kindCnt;
        //         }
        //     }
        // }
        // return kindCnt;
        int curKindCnt = hashvalcnt.size();
        if (curKindCnt > halfCnt)
            return halfCnt;

        return curKindCnt;
    }
};

