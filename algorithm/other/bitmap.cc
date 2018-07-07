#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <set>
#include <assert.h>
#include "random_leveldb.h"

// 位数组的字个数
#define WORD_NUM ((int)pow(10.0,7.0))
// 1字=4字节=32bit
// 最大可以表示的数据
#define WORD_BITS (1*4*8)

// bitmap中最大能存储的数据
#define MAX_SAVE (WORD_NUM*WORD_BITS)

// 在bitmap中添加num(设置第num位为1)
void SetBitMap(uint32_t bitmap[], uint32_t num)
{
    assert(num <= MAX_SAVE);
    int idx = num / WORD_BITS;
    bitmap[idx] |= (1 << (num % WORD_BITS)); 
}
// 从bitmap中删除num(设置第num位为0)
void ClearBitMap(uint32_t bitmap[], uint32_t num)
{
    assert(num <= MAX_SAVE);
    int idx = num / WORD_BITS;
    bitmap[idx] &= (~(1 << (num % WORD_BITS))); 
}

// 测试num是否在bitmap中
bool TestBitMap(uint32_t bitmap[], uint32_t num)
{
    assert(num <= MAX_SAVE);
    int idx = num / WORD_BITS;
    return (bitmap[idx] & (1 << (num % WORD_BITS))) != 0;
}

// 10^7 * 4Byte
// 1MB = 1024*1024 = 2^20Byte
// 生成n个[0,9999998]区间的数存储在s中
void GenerateNRadom(std::set<uint32_t>& s, const int N)
{
    leveldb::Random random(1234567);
    uint32_t num = 0;
    for (int i = 0; i < N && s.size() != N; ++i)
    {
        num = random.Uniform(9999999);
        s.insert(num);
    }
}

// 输入:一个文件,至多包含n个正整数,每个正整数都小于n,n=10^7
//      不存在重复的数,每个数据互补关联
// 输出:以升序排列的有序整数列表
// 约束:只有1MB内存可用,但是外存充足,运行时间至多允许几分钟,10s是最佳时间
int main(int argc, char* argv[])
{
    uint32_t *bitmap = (uint32_t*)malloc(sizeof(uint32_t)*WORD_NUM);
    memset(bitmap, 0, sizeof(uint32_t)*WORD_NUM);
    std::set<uint32_t> s;
    GenerateNRadom(s, (int)pow(10.0,7.0));
    for (std::set<uint32_t>::iterator it = s.begin(); it != s.end(); ++it)
    {
         SetBitMap(bitmap, *it);
    }

    for (int i = 0; i < MAX_SAVE; ++i)
    {
        if (TestBitMap(bitmap, i))
        {
            printf("%d\n", i);
        }
    }
    free(bitmap);

    // int arr[] = { 128, 1, 2, 0, 64, 4, 8, 32, 7, 5, 6, 256, 127, 55, 77 };
    // int len = sizeof(arr) / sizeof(arr[0]);

    // int bitmap[WORD_NUM];
    // memset(bitmap, 0, sizeof(int)*WORD_NUM);
    // // for (int i = 0; i < WORD_NUM; ++i)
    // // {
    // //     printf("addr-%d:%p\n", i, &bitmap[i]);
    // // }
    
    // // arr add to bitmap
    // for (int i = 0; i < len; ++i)
    // {
    //     SetBitMap(bitmap, arr[i]);
    // }
    // // ClearBitMap(bitmap, 128);
    // printf("v: %d\n", bitmap[0]);
    // // loop bit
    // for (int i = 0; i <= MAX_SAVE; ++i)
    // {
    //     // printf("(%d %d) -> ", i, TestBitMap(bitmap, i));
    //     if (TestBitMap(bitmap, i))
    //     {
    //         printf("%d ", i);
    //     }
    // }
    // printf("\n");
    
    return 0;
}