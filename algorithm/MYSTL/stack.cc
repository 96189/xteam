#include "stack.h"
#include <stdio.h>

// 进制转换
void baseConvert(long long n, int base, MYSTL::VStack<char>& s)
{
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    while (n > 0)
    {
        s.push(digit[n % base]);
        n /= base;
    }
}

// 递归版 进制转换
void R_baseConvert(long long n, int base, MYSTL::VStack<char>& s)
{
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    if (n <= 0)
    {
        return;
    }
    // go
    R_baseConvert(n/base, base, s);
    // slove
    s.push(digit[n % base]);
}

void PrintChar(char& c)
{
    printf("%c", c);
}
int main(int argc, char* argv[])
{
    // MYSTL::VStack<int> vs;
    // vs.push(1);
    // vs.push(3);
    // vs.push(5);
    // printf("top:%d\n", vs.top());
    // printf("pop:%d\n", vs.pop());
    // printf("pop:%d\n", vs.pop());
    // printf("top:%d\n", vs.top());
    MYSTL::VStack<char> vs;
    baseConvert(11, 2, vs);
    vs.reverse();
    vs.traverse(PrintChar);
    printf("\n");

    vs.clear();
    R_baseConvert(11, 2, vs);
    vs.traverse(PrintChar);
    printf("\n");
    return 0;
}