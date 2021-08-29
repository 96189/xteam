

#include <iostream>

// 只能处理正数
// 循环次数为位数
// int binary_count_1(int n) {
//     int count = 0;
//     while (n) {
//         if (n & 1) {
//             ++count;
//         }
//         n >>= 1;
//     }
//     return count;
// }

// 正负数都能处理
// 循环次数固定为32
// int binary_count_1(int n) {
//     int count = 0;
//     int flag = 1;
//     while (flag) {
//         if (flag & n) {
//             ++count;
//         }
//         flag <<= 1;
//     }
//     return count;
// }

// 7 
// 111  110     101     100
// 110  101     100      10
int binary_count_1(int n) {
    int count = 0;
    while (n) {
        ++count;
        n &= (n-1);
    }
    return count;
}


int main(int argc, char *argv[]) {
    for (int i = 1; i <= 5; ++i) {
        int n = binary_count_1(i);
        std::cout << n << std::endl;
    }
    return 0;
}

