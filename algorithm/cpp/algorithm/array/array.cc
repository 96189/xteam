
#include "array.h"

// 输入n个整数，输出其中最小的k个。

// O(k) + (n-k) * O(k)
void MinKV1(std::vector<int>& array, int k)
{
    // [0,k)  [k,len)
    unsigned int maxIdx = MaxValueIdx(array);
    for (unsigned int i = k; i < array.size(); ++i)
    {
        if (array[i] < array[maxIdx])
        {
            Swap(array[maxIdx], array[i]);
            // O(k)
            maxIdx = MaxValueIdx(array);
        }
    }
}

// O(log(k)) + (n-k) * O(log(k))
void MinKV2(std::vector<int>& array, int k)
{
    InitMaxHeap(array, k);
    for (unsigned int i = k; i < array.size(); ++i)
    {
        if (array[i] < MaxHeapTop<int>(array))
        {
            MaxHeapPush<int>(array, k, i);
        }
    }
}

// array为有序序列 diff必为有序序列
// 有序序列
// 空间复杂度 O(N)
// 时间复杂度 O(N)
bool Sum2_v1(const std::vector<int>& array, const int val, std::vector<int>& res)
{
    // 构造差值表
    std::vector<int> diff;   // O(N)
    for (unsigned int i = 0; i < array.size(); ++i)
    {
        diff.push_back(val - array[i]);
    }
    
    // 同时遍历源数据表array[i]和差值表diff[j]
    unsigned int i = 0; 
    int j = array.size()-1; 
    // O(N)
    while (i < array.size() && j >= 0)
    {
        if (array[i] > diff[j]) 
        {
            --j;
        }
        else if (array[i] < diff[j])
        {
            ++i;
        }
        else
        {
            res.push_back(array[i]);
            ++i; 
            --j;
        } 
    }
    return (res.size() == 2);
}

// 有序序列
// 时间复杂度 O(N)
// 空间复杂度 O(1)
bool Sum2_v2(const std::vector<int>& array, const int val, std::vector<int>& res)
{
    unsigned int i = 0;
    unsigned int j = array.size()-1;
    // O(N)
    while (i < j)
    {
        if (array[i] + array[j] > val)
        {
            --j;
        }
        else if (array[i] + array[j] < val)
        {
            ++i;
        }
        else
        {
            res.push_back(array[i]);
            res.push_back(array[j]);
            break;
        }
    }
    return (res.size() == 2);
}

std::vector<int> Sum2(const std::vector<int>& array, const int val)
{
    // 预处理 建立hash表
    std::unordered_map<int, int> hashTable; // diff -> valueIdx
    for (unsigned int i = 0; i < array.size(); ++i)
    {
        // hash表中找差值
        if (hashTable.find(val - array[i]) != hashTable.end())
        {
            return std::vector<int>{array[hashTable[val - array[i]]], array[i]};
        }
        // 原值存储到hash表
        hashTable[array[i]] = i;
    }
    return std::vector<int>{-1, -1};
}

// O(n^2)
// O(n)
std::vector<int> Sum3(const std::vector<int>& array, const int val)
{
    std::vector<int> res;
    for (unsigned int i = 0; i < array.size(); ++i)
    {
        auto diff = val - array[i];
        // 预处理 建立hash表
        std::unordered_map<int, int> hashTable; // diff -> valueIdx
        for (unsigned int j = 0; j < array.size(); ++j)
        {
            if (array[i] == array[j])
                continue;

            // hash表中找差值
            if (hashTable.find(diff - array[j]) != hashTable.end())
            {
                res.push_back(array[hashTable[diff - array[j]]]);
                res.push_back(array[j]);
                break;
            }
            // 原值存储到hash表
            hashTable[array[j]] = j;
        }
        if (res.size() == 2)
        {
            res.push_back(array[i]);
            break;
        }
        else 
        {
            res.clear();
        }
    }    
    return res;
}


int BinSearch(const std::vector<int>& vec, const int val)
{
    int start = 0;
    int end = vec.size() - 1;
    int mid = -1;
    while (start <= end)
    {
        mid = (start + end) / 2;
        if (val < vec[mid])
        {
            end = mid - 1;
        }
        else if (vec[mid] < val)
        {
            start = mid + 1;
        }
        else 
        {
            break;
        }
    }
    return mid;
}
