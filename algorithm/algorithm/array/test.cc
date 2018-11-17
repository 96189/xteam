
#include "array.h"
#include <assert.h>

void Test_Min_K()
{
    const int len = 10;
    const int k = 5;
    int arr[len] = {0};
    for (int i = 0; i < len; ++i)
    {
        arr[i] = rand() % MOD;
    }
    PrintArray(arr, len);
    // MinKV1(arr, len, k);
    MinKV2(arr, len, k);
    PrintArray(arr, k);
}

void Test_TwoSum_N()
{
    int array[] = { 1, 2, 4, 7, 11, 15 };
    int len = sizeof(array) / sizeof(array[0]);
    int res[2] = { 0 };
    // bool has2 = FindTwoSumN_v1(array, len, 15, res);
    bool has2 = FindTwoSumN_v2(array, len, 15, res);
    assert(has2 == true);
    PrintArray(res, 2);
}

int main(int argc, char const *argv[])
{
    // Test_Min_K();
    Test_TwoSum_N();
    return 0;
}
