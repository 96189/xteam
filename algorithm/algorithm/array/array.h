#include "pub.h"
#include <vector>
#include <unordered_map>

// 序列中最小的k个数
void MinKV1(std::vector<int>& array, int k);
void MinKV2(std::vector<int>& array, int k);

// 两数之和为n 在有序列中找到两个数字
bool Sum2_v1(const std::vector<int>& array, const int val, std::vector<int>& res);
bool Sum2_v2(const std::vector<int>& array, const int val, std::vector<int>& res);


std::vector<int> Sum2(const std::vector<int>& array, const int val);
std::vector<int> Sum3(const std::vector<int>& array, const int val);
