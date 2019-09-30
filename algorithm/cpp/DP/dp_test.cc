#include "dp_base.h"

int main(int argc, char *argv[])
{
    // for (int aim = 0; aim < 20; ++aim)
    // {
    //     printf("Reach:%d least need %d coins\n", aim, MinCoinReachAimMoney(aim));
    // }
    // int tower[5*5] = { 0 };
    // InitTower(tower, 5);
    // ShowTower(tower, 5);
    // int maxup = MaxPathSumUp(tower,5);
    // int maxdown = MaxPathSumDown(tower,5);
    // printf("%d %d\n", maxup, maxdown);

    int arr[] = {2, 2, 5, 1, 1, 3, 4, 8, 6, 7};
    int len = sizeof(arr) / sizeof(arr[0]);
    for (int i = 1; i <= len; ++i)
    {
        int maxSeq = LIS(arr, i);
        printf("%d\n", maxSeq);
    }
    return 0;
}