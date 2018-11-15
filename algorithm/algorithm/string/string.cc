
#include "string.h"

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