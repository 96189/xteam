
#include "string.h"
#include <assert.h>

// 字符串全排列
void Permutations(char *seq, int start, int end)
{
    if (start == end)
    {
        printf("%s\n", seq);
        return;
    }

    // 遍历字符 每次固定一个字符
    for (int i = start; i <= end; ++i)
    {
        Swap(seq[i], seq[start]);       // 交换
        Permutations(seq, start+1, end);
        Swap(seq[i], seq[start]);       // 还原
    }
}

// 字符串旋转k个字符
void RotateString(char *seq, int k)
{
    if (k <= 0) return;

    int len = strlen(seq);      // O(n)
    int mod = k % len;

    reverse(seq, 0, mod-1);     // O(M)
    reverse(seq, mod, len-1);   // O(N)
    reverse(seq, 0, len-1);     // O(M+N) = O(N)
}

// 用位运算(26bit整数表示)计算出一个签名 再用B中的字符到A里面进行查找
bool StringContain(char *a, char *b)
{
    unsigned int lenA = strlen(a);
    unsigned int lenB = strlen(b);
    assert(lenA > lenB);
    Upper(a);
    Upper(b);

    int hash = 0;   // O(1)
    // 构建hash表  O(lenA)
    for (unsigned int i = 0; i < lenA; ++i)
    {
        hash |= (1 << (a[i] - 'A'));
    }
    // hash表查找  O(lenB)
    for (unsigned int j = 0; j < lenB; ++j)
    {
        if ((hash & (1 << (b[j] - 'A'))) == 0)
        {
            return false;
        }
    }
    return true;
}

int Str2Int(const char* str)
{
    // int的最大最小值
    static const int MAX_INT = (int)((unsigned)~0 >> 1);
    static const int MIN_INT = (int)((unsigned)~0 >> 1) - 1;

    if (!str) return 0;

    // 跳过头部空格
    while (isspace(*str)) ++str;

    // 默认为正数
    int sign = 1;
    if (*str == '+' || *str == '-')
    {
        if (*str == '-') sign = -1;
        ++str;
    }   

    // 
    unsigned int n = 0;
    while (isdigit(*str))
    {
        int ch = *str - '0';
        // 正数溢出
        if (sign > 0 && 
            (n > MAX_INT/10 || (n == MAX_INT/10 && ch > MAX_INT % 10)))
        {
            n = MAX_INT;
            break;
        }
        // 负数溢出
        else if (sign < 0 &&
            (n > MIN_INT/10 || (n == MIN_INT/10 && ch > MIN_INT % 10)))
        {
            n = MIN_INT;
            break;
        }

        // 转换规则
        n = n * 10 + ch;
        ++str;
    }
    return sign > 0 ? n : -n;
}