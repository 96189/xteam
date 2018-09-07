#ifndef _STRING_H_
#define _STRING_H_

#include "vector.h"

namespace MYSTL 
{

class String
{
private:
    Vector<char> val_;

public:
// 构造析构
    String()
    {
        val_.clear();
    }
    String(const char *str)
        : val_(str, strlen(str))
    {

    }
    String(const char *str, int lo, int hi)
        : val_(str + lo, hi - lo)
    {

    }
    String(const String &rhs)
    {
        if (this != &rhs)
        {
            val_.clear();
            val_(rhs.val_);
        }
    }
    String& operator=(const String &rhs)
    {
        val_ = rhs.val_;
    }
    ~String()
    {
        val_.clear();
    }
// 读操作
    int length() const 
    {
        return val_.size();
    }
    char charAt(int i) const 
    {
        return val_[i];
    }
    String substr(int i, int k) const 
    {

    }
    String prefix(int k) const 
    {

    }
    String suffix(int k) const 
    {

    }
    bool operator==(const String &rhs)
    {

    }
// 写操作
    String& operator+(const String &rhs)
    {

    }
// 
    int match1(const String &pattern)
    {
        int n = this->length();
        int m = pattern.length();
        int i;
        // 文本串
        // 失配后文本串每次回退到i位置 i按照1前进
        for (i = 0; i < n; ++i)
        {
            // 模式串
            // 失配后模式串每次回退到0位置
            for (int j = 0; j < m; ++j)
            {
                // 匹配
                if (val_[i + j] == pattern[j])
                {
                    // nothing
                }
                // 失配
                else 
                {
                    break;
                }
            }
            // j代表匹配字符的个数 刚好匹配立刻返回
            if (j == m - 1) break;
        }
        return i;
    }
    int match2(const String &pattern)
    {
        int n = this->length();
        int m = pattern.length();
        int i = 0;
        int j = 0;
        while (i < n && j < m)
        {
            if (val_[i] == pattern[j])
            {
                ++i;
                ++j;
            }
            else 
            {
                j = 0;  
                i = i - j + 1;
            }
        }        
    }
};

};

#endif