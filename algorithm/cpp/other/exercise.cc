#include <stdio.h>
#include <assert.h>

// 编程珠玑习题

// abcdefgh -> defghabc
// len = 8, i = n, 长度为8的字符串向左旋转3位

void ShowData(int data[], int len)
{
    for (int idx = 0; idx < len; ++idx)
    {
        printf("%d", data[idx]);
    }
    printf("\n");
}

// 旋转数组
void RotateData(int data[], int len, int n)
{
    assert(n <= len);
    for (int i = 1; i <= n; ++i)
    {
        int idx = 1;
        int tmp = data[0];
        for (; idx < len; ++idx)
        {
            data[idx - 1] = data[idx];
        }
        data[idx - 1] = tmp;
    }
}
// 
void RotateDataMod(int data[], int len, int n, int index)
{

}
// Equivalent conversion
void RotateDataEC()
{

}
// 二分查找(递归)
// 已排序序列
int BinarySearch(int data[], int start, int end, int val)
{
    if (start > end)
    {
        return -1;
    }
    int mid = (start + end) / 2;
    if (val == data[mid])
    {
        return mid; 
    }
    else if (val > data[mid])
    {
        return BinarySearch(data, mid+1, end, val);
    }
    else 
    {
        return BinarySearch(data, start, mid-1, val);
    }
}

// 循环的二分查找
int BinarySearchLoop(int data[], int len, int val)
{
    int start = 0;
    int end = len-1;
    int mid = 0;
    int idx = -1;
    while (start <= end)
    {
        mid = (start + end) / 2;
        if (data[mid] == val)
        {
            idx = mid;
            break;
        }
        else if (val < data[mid])
        {
            end = mid-1;
        }
        else 
        {
            start = mid+1;
        }
    }
    return idx;
}

// 寻找旋转数组中最大或者最小的值
#define MAX 1
#define MIN 0
// data [start, end]
int RotateExtremeValue(int data[], int start, int end, int flag)
{
    if (end - start == 1 && data[start] > data[end])
    {
        if (flag == MAX)
        {
            return data[start];
        }
        else if (flag == MIN)
        {
            return data[end];
        }
    }
    int mid = (start + end) / 2;
    if (data[start] < data[mid])
    {
        return RotateExtremeValue(data, mid, end, flag);
    }
    else 
    {
        return RotateExtremeValue(data, start, mid, flag);
    }
}

int main(int argc, char* argv[])
{
    int data[] = { 1, 2, 2, 3, 3, 4, 5, 6, 7, 8, 9 };
    int len = sizeof(data) / sizeof(data[0]);
    // ShowData(data, len);
    // RotateData(data, len, 2);
    // ShowData(data, len);
    
    // int idx = BinarySearch(data, 0, len-1, 5);
    // assert(data[idx] == 5);
    // idx = BinarySearch(data, 0, len-1, 55);
    // assert(idx == -1);
    // idx = BinarySearchLoop(data, len, 5);
    // assert(data[idx] == 5);
    // idx = BinarySearchLoop(data, len, 55);
    // assert(idx == -1);

    for (int i = 1; i < len; ++i)
    {
        RotateData(data, len, 1);
        int v = RotateExtremeValue(data, 0, len-1, MAX);
        printf("MAX:%d\n", v);
        v = RotateExtremeValue(data, 0, len-1, MIN);
        printf("MIN:%d\n", v);
    }

    return 0;
}