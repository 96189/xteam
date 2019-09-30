#include "choicecard.h"
#include <algorithm>
// group1不重复的元素
// group2重复的元素
void Deduplicate(vector<BYTE>& cards, vector<BYTE>& group1, vector<BYTE>& group2)
{
	group1.assign(cards.begin(), cards.end());
	int len = group1.size();
	int value = group1[0];
	for (int i = 1; i < len; ++i)
	{
		if (value == group1[i])
		{
			group2.push_back(group1[i]);
		}
		value = group1[i];
	}
	group1.erase(unique(group1.begin(), group1.end()), group1.end());
	if (group1.size() == 1)
	{
		group2.push_back(group1[0]);
		group1.clear();
	}
}
// 基于已经按照升序排列的数组
// count[6,7]
void DivideCard(vector<BYTE>& cards, int count, vector<BYTE>& group1, vector<BYTE>& group2)
{
	Deduplicate(cards, group1, group2);
	int nCards = group1.size();
	int start = -1;
	// 牌数大于5才可能是顺子
    // 4, 5, 7, 8, 9, 10, 11
	if (nCards >= 5)
	{
		// nCards [5,6,7]
		for (int i = nCards; i >= 5; --i)
		{
			if (group1[i - 1] - group1[i - 5] == 4)
			{
				start = i - 5;//[0, 2]
				break;
			}
		}
		if (start != -1)
		{
			// 移动group1的头尾到group2,并删除group1中移除的头尾元素
			if (start > 0)
			{
				for (int i = start - 1; i >= 0; --i)
				{
					group2.push_back(group1[i]);
				}
			}
			if (start + 5 < group1.size())
			{
				for (int i = start + 5; i < group1.size(); ++i)
				{
					group2.push_back(group1[i]);
				}
			}
			vector<BYTE> tmp;
			// [start, start + 5)
			tmp.assign(group1.begin() + start, group1.begin() + start + 5);
			tmp.swap(group1);
			tmp.clear();
		}
	}
}
// 顺子
bool IsStraight(vector<BYTE>& aimCards)
{
	bool isStraight = false;
	if (aimCards.size() >= 5)
	{
		BYTE curValue = (aimCards[0]);
		for (int i = 1; i < aimCards.size(); i++)
		{
			if (curValue + 0x01 == (aimCards[i]))
			{
				isStraight = true;
				curValue = (aimCards[i]);
			}
			else
			{
				isStraight = false;
				break;
			}
		}
	}
	return isStraight;
}

struct Less
{
	bool operator()(BYTE card1, BYTE card2)
	{
		return (card1 < card2);
	}
};

// 是否顺子
// 5张以下都不是顺子
bool IsShunZhi(vector<BYTE>& cardlist, BYTE cardcnt, vector<BYTE>& selectedCards)
{
	bool isShunzi = false;
	// 6 7需要选牌
    if (cardcnt <= 7 && cardcnt >= 5)
	{
		vector<BYTE> cards;
		vector<BYTE> aimCards;
		vector<BYTE> extraCards;
		for (int i = 0; i < cardcnt; i++)
		{
			cards.push_back(cardlist[i]);
		}
		sort(cards.begin(), cards.end(), Less());
		DivideCard(cards, cardcnt, aimCards, extraCards);
		if (IsStraight(aimCards) && aimCards.size() == 5)
		{
			isShunzi = true;
		}
		else
		{
			isShunzi = false;
		}
        // 先保存最大的顺子牌
		for (vector<BYTE>::iterator it = aimCards.begin(); it != aimCards.end(); ++it)
		{
			selectedCards.push_back(*it);
		}
        // 然后保存其余牌
		for (vector<BYTE>::iterator it = extraCards.begin(); it != extraCards.end(); ++it)
		{
            selectedCards.push_back(*it);
		}
	}
	else
	{
		isShunzi = false;
	}
	return isShunzi;
}