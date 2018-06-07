#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include<boost/unordered_set.hpp>
#include <iostream>

void PrintCArray(int arr[], int len)
{
    for (int i = 0; i < len; ++i)
    {
        printf("%d -> ", arr[i]);
    }
    printf("\n\n");
}

void Swap(int& v1, int& v2)
{
    int t = v1;
    v1 = v2;
    v2 = t;
}

// 模拟洗牌
void FisherYatesShuffle(int arr[], int len)
{
    for (int i = len - 1; i >= 0; --i)
    {
        // 在剩余区域取随机值
        int index = rand() % (i + 1);
        Swap(arr[i], arr[index]);
    }
    // for (int i = 0; i < len; ++i)
    // {
    //     int index = rand() % (len - i);
    //     Swap(arr[i], arr[index]);    
    // }
}

// 利用boost::unordered_set实现洗牌操作
void Shuffle(int arr[], int n, boost::unordered_set<int>& s)
{
    while (s.size() < n)
    {
        int index = rand() % n;
        s.insert(arr[index]);
    }
}

///////////////////////////////////////////////////////////////////////
// STL stl_algo.h random_shuffle
// random_shuffle

template <class _RandomAccessIter>
inline void random_shuffle(_RandomAccessIter __first,
                           _RandomAccessIter __last) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    // FisherYates算法
    // [m, k, n] m <= k && k <= n
    // 将第k个元素和[m,k]之间的元素交换
    iter_swap(__i, __first + __random_number((__i - __first) + 1));
}

template <class _RandomAccessIter, class _RandomNumberGenerator>
void random_shuffle(_RandomAccessIter __first, _RandomAccessIter __last,
                    _RandomNumberGenerator& __rand) {
  __STL_REQUIRES(_RandomAccessIter, _Mutable_RandomAccessIterator);
  if (__first == __last) return;
  for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
    iter_swap(__i, __first + __rand((__i - __first) + 1));
}
///////////////////////////////////////////////////////////////////////

// 模拟发牌(每张牌抽到的概率一样)
// 蓄水池抽样算法
// 数据规模为n,要采样的数量为k
// 
void ReservoirSampling(int dataSet[], int n, int cards[], int k)
{
    // 将数据的前k个元素存储到数组(蓄水池)
    for (int i = 0; i < k; ++i)
    {
        cards[i] = dataSet[i];
    }
    // 从第k+1开始
    // 以k/n的概率决定元素是否被替换到数组中
    for (int j = k; j < n; ++j)
    {
        // [k,n]的随机数
        int index = rand() % j;
        // 随机数index落在[1,k]范围,则替换
        if (index <= (k - 1))
        {
            cards[index] = dataSet[j]; 
        }
    }
}

// 除去大小王的一副牌
void InitCards(int arr[], int len)
{
    assert(52 == len);
    for (int i = 0; i < len; ++i)
    {
        arr[i] = i + 1;
    }
}

int main(int argc, char* argv[])
{
    int arr[52] = {0};
    InitCards(arr, 52);
    // PrintCArray(arr, 52);
    // for (int i = 0; i < 10; ++i)
    // {
    //     FisherYatesShuffle(arr, 52);
    //     PrintCArray(arr, 52);
    // }

    // int cards[7] = {0};
    // // 52张牌中抽取7张牌
    // for (int i = 0; i < 10; ++i)
    // {
    //     ReservoirSampling(arr, 52, cards, 7);
    //     PrintCArray(cards, 7);
    // }

    for (int i = 0; i < 10; ++i)
    {
        boost::unordered_set<int> s;
        s.clear();
        Shuffle(arr, 52, s);
        
        for (boost::unordered_set<int>::iterator it = s.begin(); it != s.end(); ++it)
        {
            std::cout << *it << " -> ";
        }
        std::cout << "\n";
    }
    return 0;
}