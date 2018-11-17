
//

#include "string.h"
#include <assert.h>

void Test_Perm()
{
    char str[64] = {'\0'};
    memcpy(str, "abc", 3);
    Permutations(str, 0, strlen(str)-1);
}

void Test_Rotate()
{
    const char *data = "abcdef";
    char str[64] = {'\0'};
    memcpy(str, data, strlen(data));
    printf("%s\n", str);
    for (unsigned int i = 0; i < strlen(str); ++i)
    {
        RotateString(str, i);
        printf("rotate %d -> %s\n", i, str);
    }
}

void Test_Contain()
{
    const char* a = "ABCD";
    const char* b = "ABC";
    const char* c = "ACE";
    char src[64] = {'\0'};
    memcpy(src, a, strlen(a));

    char pattern[64] = {'\0'};
    memcpy(pattern, b, strlen(b));
    bool bContain = StringContain(src, pattern);
    assert(bContain == true);
    
    memset(pattern, 0, 64);
    memcpy(pattern, c, strlen(c));
    bContain = StringContain(src, pattern);
    assert(bContain == false);
}

void Test_Str2Int()
{
    const char* num = "-32768";
    const char* num2 = "96189";
    const char* num3 = "2147483647";
    const char* num4 = "-2147483648";
    printf("%d\n", Str2Int(num));
    printf("%d\n", Str2Int(num2));
    printf("%d\n", Str2Int(num3));
    printf("%d\n", Str2Int(num4));
}

int main(int argc, char const *argv[])
{
    // Test_Perm();
    // Test_Rotate();
    // Test_Contain();
    Test_Str2Int();
    return 0;
}
