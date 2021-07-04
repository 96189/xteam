

// input -> { 2, 3, 1, 0, 2, 5, 3 }
// output -> 2 3

#include <vector>
#include <iostream>
#include <algorithm>

// 解法1
bool get_duplicate_num(std::vector<int> &nums, std::vector<int> &result) {
    // O(nlog(n))
    std::sort(nums.begin(), nums.end());
    
    // O(n)
    int v = nums[0];
    size_t i = 1;
    while (i < nums.size()) {
        if (nums[i] == v) {
            result.push_back(v);
            while (++i < nums.size() && nums[i] == v) {
                
            }
        } else {
            v = nums[i];
            ++i;
        }
    }
    return result.size() > 0;
}

// 解法2 hashmap

// 解法3

void print_vector(const std::vector<int> &nums) {
    std::for_each(nums.begin(), nums.end(), [](const int v) {
        std::cout << v << " ";
    });
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    std::vector<int> nums = { 2, 3, 1, 0, 2, 5, 3 };
    std::vector<int> result;
    get_duplicate_num(nums, result);
    print_vector(result);

    // 
    std::vector<int> nums1 = { 2, 2, 2, 2, 2, 2, 2 };
    std::vector<int> result1;
    get_duplicate_num(nums1, result1);
    print_vector(result1);

    // 
    std::vector<int> nums2 = { 1, 2, 3, 4, 5, 6, 7 };
    std::vector<int> result2;
    get_duplicate_num(nums2, result2);
    print_vector(result2);
    return 0;
}