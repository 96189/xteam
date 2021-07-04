

// 求斐波那契数列的第n项
// 定义
//  f(0) = 0
//  f(1) = 1
//  f(n) = f(n-1) + f(n-2)

#include <iostream>

int fibnacci(int n) {
    int v0 = 0;
    int v1 = 1;
    if (0 == n) {
        return v0;
    }
    if (1 == n) {
        return v1;
    }
    int i = 2;
    int vi = 0;
    while (i++ <= n) {
        vi = v0 + v1;
        v0 = v1;
        v1 = vi;
    }
    return vi;
}

// 青蛙跳台阶问题
// 一只青蛙一次可以跳上1级台阶，也可以跳上2级台阶。求该青蛙跳上一个n级台阶总共有多少种跳法
// 0、1、1、2、3、5、8、13、21、34
int main(int argc, char *argv[]) {
    int n = fibnacci(9);
    std::cout << n << std::endl;
}