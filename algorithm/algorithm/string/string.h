
#include "pub.h"
#include <string.h>

// seq [start, end]
void Permutations(char *seq, int start, int end);

void RotateString(char *seq, int k);

// 判断b中的所有字符是否都在a中
bool StringContain(char *a, char *b);

// 字符串转整数int
int Str2Int(const char* str);

// TODO
int Manacher(const char* str);
