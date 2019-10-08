/*
 * @lc app=leetcode.cn id=682 lang=cpp
 *
 * [682] 棒球比赛
 */
class Solution {
public:
    bool strDigit(string &str)
    {
        if (str.empty())
            return false;
        int i = 0;
        int len = str.size();
        if (str[i] == '-')
            ++i;
        for (; i < len; ++i)
        {
            if (!isdigit(str[i]))
                return false;
        }
        return true;
    }
    int calPoints(vector<string>& ops) {
        if (ops.empty())
            return 0;
        vector<int> nums;
        int sum = 0;
        for (auto &op : ops)
        {
            if (strDigit(op))
            {
                nums.push_back(std::stoi(op));
                sum += nums.back();
            }
            else if (op == "C")
            {
                if (!nums.empty())
                {
                    sum -= nums.back();
                    nums.pop_back();
                }
            }
            else if (op == "D")
            {
                if (!nums.empty())
                {
                    nums.push_back(nums.back() * 2);
                    sum += nums.back();
                }
            }
            else if (op == "+")
            {
                int idx = nums.size() - 1;
                if (idx - 1 >= 0)
                {
                    nums.push_back(nums[idx] + nums[idx - 1]);
                    sum += nums.back();
                }
            }
        }
        return sum;
    }
};

