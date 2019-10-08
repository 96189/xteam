/*
 * @lc app=leetcode.cn id=811 lang=cpp
 *
 * [811] 区间子数组个数
 */
class Solution {
public:
    vector<string> subdomainVisits(vector<string>& cpdomains) {
        vector<string> allcpddomains;
        // 建表
        map<string, int> domainCnt;
        for (auto &item : cpdomains)
        {
            auto pos = item.find_first_of(" ");
            if (pos == string::npos)
                continue;
            string domain = item.substr(pos + 1, item.size() - pos);
            int cnt = stoi(item.substr(0, pos));
            domainCnt[domain] = cnt;
        }
        // 遍历表 表项分割
        for (auto &kvitem : domainCnt)
        {
            string domain = kvitem.first;
            auto pos = domain.find_first_of(".");
            if (pos != string::npos)
            {
                string subdomain = domain.substr(pos + 1, domain.size() - pos);
                //
                if (domainCnt[subdomain])
                    domainCnt[subdomain] += kvitem.second;
                else
                    domainCnt[subdomain] = kvitem.second;
                domain = subdomain;
                pos = domain.find_first_of(".");
            }
        }
        for (auto &kvitem : domainCnt)
        {
            allcpddomains.push_back(to_string(kvitem.second) + " " + kvitem.first);
        }
        return allcpddomains;
    }
};

