
#include <vector>
#include <algorithm>
#include <iostream>

bool odd(int v) {
    return (v & 1) ? true : false;
}

bool even(int v) {
    return !odd(v);
}

void swap(int &v1, int &v2) {
    int t = v1;
    v1 = v2;
    v2 = t;
}
void reorder_use_oddeven(std::vector<int> &nums) {
    int l = 0;
    int r = nums.size() - 1;
    while (l < r) {
        while (l < r && odd(nums[l])) {
            ++l;
        }
        while (l < r && even(nums[r])) {
            --r;
        }
        if (l < r) {
            swap(nums[l], nums[r]);
            ++l;
            --r;
        }
    }
}

void print_vector(const std::vector<int> &nums) {
    for_each(nums.begin(), nums.end(), [](const int v) {
        std::cout << v << std::endl;
    });
}


int main(int argc, char *argv[]) {
    std::vector<int> nums = { 2, 4, 6, 1, 3, 5 };
    reorder_use_oddeven(nums);
    print_vector(nums);
    return 0;
}