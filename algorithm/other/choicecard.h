#include <vector>
#include <stdio.h>

using namespace std;

typedef unsigned char BYTE;

// 去重,将cards中不重复的部分复制到group1,重复的部分复制到group2
void Deduplicate(vector<BYTE>& cards, vector<BYTE>& group1, vector<BYTE>& group2);
// 分牌,group1中保存最大的顺子,group2保存剩余牌
void DivideCard(vector<BYTE>& cards, int count, vector<BYTE>& group1,vector<BYTE>& group2);
// 判断是否是顺子
bool IsStraight(vector<BYTE>& aimCards);
// 对于给定的cardlist cardcnt>5 选取最大的五张顺子保存到selectedCards其余牌依次存储到selectedCards
bool IsShunZhi(vector<BYTE>& cardlist, BYTE cardcnt, vector<BYTE>& selectedCards);