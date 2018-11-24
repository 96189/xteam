
//
#include "pub.h"

// // --------------------------------------------------------------------- //
void Upper(char* str)
{
    unsigned int len = strlen(str);
    for (unsigned int i = 0; i < len; ++i)
    {
        str[i] = toupper(str[i]);
    }
}
