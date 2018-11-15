
//

#include "string.h"
#include <string.h>

void Test_Perm()
{
    char str[64] = {'\0'};
    memcpy(str, "abc", 3);
    Permutations(str, 0, strlen(str)-1);
}

int main(int argc, char const *argv[])
{
    Test_Perm();
    return 0;
}
