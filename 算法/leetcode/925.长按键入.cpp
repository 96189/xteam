/*
 * @lc app=leetcode.cn id=925 lang=cpp
 *
 * [925] 长按键入
 */
class Solution {
public:
    bool isLongPressedName(string name, string typed) {
        if (name.length() > typed.length())
            return false;
        int s = 0;
        int i = 0;
        int e = 0;
        int j = 0;
        int namelen = name.length();
        int typedlen = typed.length();
        if (namelen > 0 && typedlen > 0 && name[namelen - 1] != typed[typedlen - 1])
        {
            return false;
        }
        for (e = s + 1; e < namelen; ++e)
        {
            if (name[s] == name[e])
                continue;
            int diffName = e - s;
            s = e;
            for (j = i + 1; j < typedlen; ++j)
            {
                if (typed[i] == typed[j])
                    continue;
                int diffTyped = j - i;
                i = j;
                if (name[s] != typed[i])
                {
                    diffTyped = 0;
                }
                if (diffName > diffTyped)
                    return false;
                break;
            }
        }
        return true;
    }
};

