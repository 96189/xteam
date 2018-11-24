
#include "array.h"
#include <assert.h>

#define MOD 100

void Test_Min_K()
{
    const int len = 10;
    const int k = 5;
    std::vector<int> arr;
    for (int i = 0; i < len; ++i)
    {
        arr.push_back(rand() % MOD);
    }
    PrintVector(arr);
    MinKV1(arr, k);
    MinKV2(arr, k);
    PrintVector(arr);
}

void Test_TwoSum_N()
{
    std::vector<int> array = { 1, 2, 3, 4, 6, 7, 11, 15 };
    std::vector<int> res;
    // bool has2 = Sum2_v1(array, 15, res);
    // bool has2 = Sum2_v2(array, 15, res);
    // assert(has2 == true);
    // PrintVector(res);

    // std::vector<int> vals = Sum2(array, 15);
    std::vector<int> vals = Sum3(array, 12);
    PrintVector(vals);
}

int main(int argc, char const *argv[])
{
    // Test_Min_K();
    Test_TwoSum_N();
    return 0;
}
