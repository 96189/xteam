#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
// 进制转换
void baseConvert(long long n, int base, MYSTL::Stack<char>& s)
{
    static char digit[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    while (n > 0)
    {
        s.push(digit[n % base]);
        n /= base;
    }
}

// 递归版 进制转换
void R_baseConvert(long long n, int base, MYSTL::Stack<char>& s)
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

// 检查表达式括号是否匹配
bool IsMatchBrackets(const char* text)
{
    assert(text);
    MYSTL::Stack<char> s;
    while (*text != '\0')
    {
        // if (*text == '(' || *text == '[' || *text == '{')
        // {
        //     s.push(*text);
        // }
        // else if (*text == ')')
        // {
        //     if (!s.empty() && s.top() == '(')
        //     {
        //          s.pop();
        //     }
        //     else 
        //     {
        //         return false;
        //     }
        // }
        // else if (*text == ']')
        // {
        //     if (!s.empty() && s.top() == '[')
        //     {
        //          s.pop();
        //     }
        //     else 
        //     {
        //         return false;
        //     }
        // }
        // else if (*text == '}')
        // {
        //     if (!s.empty() && s.top() == '{')
        //     {
        //          s.pop();
        //     }
        //     else 
        //     {
        //         return false;
        //     }
        // }
        // else 
        // {
        //     // nothing
        // }
        switch (*text)
        {
            case '(': case '[': case '{': s.push(*text); break;
            case ')':
            {
                if (s.empty() || s.pop() != '(')
                    return false;
                break;
            }
            case ']':
            {
                if (s.empty() || s.pop() != '[')
                    return false;
                break;
            }
            case '}':
            {
                if (s.empty() || s.pop() != '{')
                    return false;
                break;
            }
            default:
                break;
        }
        ++text;
    }
    return s.empty();
}
void PrintInt(int& val)
{
    printf("%d ", val);
}
// 栈混洗
void StackShuffling(MYSTL::Stack<int>& from, MYSTL::Stack<int>& to)
{
    MYSTL::Stack<int> s;
    int n = 0;
    while (!from.empty() || !s.empty())
    {
        if (!from.empty())
        {
            n = rand() % from.size();
            if (n == 0 && from.size() > 0)
            {
                s.push(from.pop());
            }
            while (n--)
            {
                s.push(from.pop());
            }
        }
        if (!s.empty())
        {
            n = rand() % s.size();
            if (n == 0 && s.size() > 0)
            {
                to.push(s.pop());
            }
            while (n--)
            {
                to.push(s.pop());
            }
        }
    }
}

// 栈混洗生成的序列检验

// 中缀表达式转后缀表达式(逆波兰表达式RPN)

// 逆波兰表达式计算值

void PrintChar(char& c)
{
    printf("%c", c);
}

int main(int argc, char* argv[])
{
    // stack api测试
    // MYSTL::Stack<int> s;
    // s.push(1);
    // s.push(3);
    // s.push(5);
    // printf("top:%d\n", s.top());
    // printf("pop:%d\n", s.pop());
    // printf("pop:%d\n", s.pop());
    // printf("top:%d\n", s.top());

    // 进制转换测试
    // MYSTL::Stack<char> s;
    // baseConvert(11, 2, s);
    // s.reverse();
    // s.traverse(PrintChar);
    // printf("\n");

    // s.clear();
    // R_baseConvert(11, 2, s);
    // s.traverse(PrintChar);
    // printf("\n");

    // 括号匹配测试
    // // assert(IsMatchBrackets("c + ((a + b) * c + (c + b) * a) + b") == true);
    // assert(IsMatchBrackets("(()())") == true);
    // // assert(IsMatchBrackets("(a + b) + c) + (d + a)") == false);
    // assert(IsMatchBrackets("())()") == false);
    // assert(IsMatchBrackets("{([)(])}") == false);
    // assert(IsMatchBrackets("{([])}") == true);

    // 栈混洗
    MYSTL::Stack<int> from;
    for (int i = 5; i >= 0; --i)
    {
        from.push(i);
    }
    MYSTL::Stack<int> to;
    StackShuffling(from, to);
    to.traverse(PrintInt);
    return 0;
}