
// input    { 3, 4, 5, 1, 2 }
// output   1
// 时间复杂度 < O(n)

#include <vector>
#include <iostream>

int min_value(const std::vector<int> &nums) {
    int l = 0;
    int r = nums.size() - 1;
    while (l < r) {
        int mid = (l + r) / 2;
        if (nums[mid] > nums[r]) {
            l = mid + 1;
        } else if (nums[mid] == nums[r]) {
            r = r - 1;
        } else {
            r = mid;
        }
    }
    return nums[l];
}

int main(int argc, char *argv[]) {
    std::vector<int> nums = { 3, 4, 5, 1, 2 };
    int v = min_value(nums);
    std::cout << v << std::endl;
    std::vector<int> nums1 = { 1, 2, 3, 4, 5 };
    int v1 = min_value(nums1);
    std::cout << v1 << std::endl;
    std::vector<int> nums2 = { 5, 1, 2, 3, 4 };
    int v2 = min_value(nums2);
    std::cout << v2 << std::endl;
    return 0;
}
