/*
 * @lc app=leetcode.cn id=929 lang=cpp
 *
 * [929] 独特的电子邮件地址
 */
class Solution {
public:
    int numUniqueEmails(vector<string>& emails) {
        unordered_map<string, set<string>> domianlocals;
        // 按照域名分类
        for (auto &email : emails)
        {
            auto pos = email.find("@");
            if (pos == string::npos)
                continue;
            auto local = email.substr(0, pos);
            auto domain = email.substr(pos + 1);
            string validlocal = "";
            for (size_t i = 0; i < local.length(); ++i)
            {
                if (local[i] == '+')
                    break;
                if (local[i] != '.')
                {
                    validlocal.push_back(local[i]);
                }
            }
            domianlocals[domain].insert(validlocal);
        }
        // 统计每一类的符合条件的本地名称
        int cnt = 0;
        for (auto &kvItem : domianlocals)
        {
            cnt += kvItem.second.size();
        }
        return cnt;
    }
};

