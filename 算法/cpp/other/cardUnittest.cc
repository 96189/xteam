#include <iostream>
#include <gtest/gtest.h>
#include "choicecard.h"

using testing::Test;

class TestCard : public Test
{
    void SetUp()
    {
        cout << "TestCard::SetUp" << endl;
    }
    void TearDown()
    {
        cout << "TestCard::TearDown" << endl;
    }
};

TEST_F(TestCard, TestDeduplicate)
{
	// BYTE arr[] = { 3, 3, 4, 4, 5, 5, 8 };
    // BYTE result1[] = { 3, 4, 5, 8 };
    // BYTE result2[] = { 3, 4, 5 };
	BYTE arr[] = { 3, 3, 3, 3, 3, 3, 3 };
    BYTE result1[] = { };
    BYTE result2[] = { 3, 3, 3, 3, 3, 3, 3 };
	BYTE len = sizeof(arr) / sizeof(arr[0]);
	std::vector<BYTE> cards(arr, arr + len);
    std::vector<BYTE> group1;
    std::vector<BYTE> group2;
    Deduplicate(cards, group1, group2);
	for (int i = 0; i < group1.size(); ++i)
	{
		ASSERT_EQ(group1[i], result1[i]);
	}    
	for (int i = 0; i < group2.size(); ++i)
	{
		ASSERT_EQ(group2[i], result2[i]);
	} 
}
TEST_F(TestCard, TestDivideCard)
{
    // 顺子在中间
	// BYTE arr[] = { 2, 5, 6, 7, 8, 9, 11 };
    // BYTE result1[] = { 5, 6, 7, 8, 9 };
    // BYTE result2[] = { 2, 11 };

    // 顺子在左边
	// BYTE arr[] = { 4, 5, 6, 7, 8, 10, 11 };
    // BYTE result1[] = { 4, 5, 6, 7, 8 };
    // BYTE result2[] = { 10, 11 };

    // 顺子在右边
	// BYTE arr[] = { 4, 5, 7, 8, 9, 10, 11 };
    // BYTE result1[] = { 7, 8, 9, 10, 11 };
    // BYTE result2[] = { 5, 4 };

    // 顺子在右边
	// BYTE arr[] = { 5, 5, 6, 7, 8, 9, 10 };
    // BYTE result1[] = { 6, 7, 8, 9, 10 };
    // BYTE result2[] = { 5, 5 };

    // 顺子在左边
	// BYTE arr[] = { 4, 5, 6, 7, 8, 10, 10 };
    // BYTE result1[] = { 4, 5, 6, 7, 8 };
    // BYTE result2[] = { 10, 10 };

    // 顺子在中间
	// BYTE arr[] = { 2, 5, 6, 7, 7, 8, 9 };
    // BYTE result1[] = { 5, 6, 7, 8, 9 };
    // BYTE result2[] = { 7, 2 };

	BYTE arr[] = { 8, 9, 10, 11, 12, 13, 14 };
    BYTE result1[] = { 10, 11, 12, 13, 14 };
    BYTE result2[] = { 9, 8 };
	BYTE len = sizeof(arr) / sizeof(arr[0]);
	std::vector<BYTE> cards(arr, arr + len);
    std::vector<BYTE> group1;
    std::vector<BYTE> group2;

    DivideCard(cards, cards.size(), group1, group2);
	for (int i = 0; i < group1.size(); ++i)
	{
		ASSERT_EQ(group1[i], result1[i]);
	}    
	for (int i = 0; i < group2.size(); ++i)
	{
		ASSERT_EQ(group2[i], result2[i]);
	}
}
TEST_F(TestCard, TestIsShunZhi)
{
    // BYTE arr[] = { 8, 9, 10, 11, 12, 13, 14 };
    // BYTE result[] = { 10, 11, 12, 13, 14, 9, 8 };

    // BYTE arr[] = { 2, 5, 6, 7, 7, 8, 9 };
    // BYTE result[] = { 5, 6, 7, 8, 9, 7, 2 };

	// BYTE arr[] = { 2, 5, 6, 7, 8, 9, 11 };
    // BYTE result[] = { 5, 6, 7, 8, 9, 2, 11 };

	// BYTE arr[] = { 4, 5, 6, 7, 8, 10, 11 };
    // BYTE result[] = { 4, 5, 6, 7, 8, 10, 11 };

	// BYTE arr[] = { 4, 5, 7, 8, 9, 10, 11 };
    // BYTE result[] = { 7, 8, 9, 10, 11, 5, 4 };

	// BYTE arr[] = { 5, 5, 6, 7, 8, 9, 10 };
    // BYTE result[] = { 6, 7, 8, 9, 10, 5, 5 };

	// BYTE arr[] = { 4, 5, 6, 7, 8, 10, 10 };
    // BYTE result[] = { 4, 5, 6, 7, 8, 10, 10 };

	BYTE arr[] = { 2, 5, 6, 7, 7, 8, 9 };
    BYTE result[] = { 5, 6, 7, 8, 9, 7, 2 };

    // BYTE arr[] = { 5, 5, 6, 6, 7, 7, 9 };
    // BYTE result[] = { 5, 6, 7, 9, 7, 6, 5 };

    BYTE len = sizeof(arr) / sizeof(arr[0]);
    std::vector<BYTE> cards(arr, arr + len);
    std::vector<BYTE> selectedCards;    
    bool shunzi = IsShunZhi(cards, cards.size(), selectedCards);
    ASSERT_EQ(shunzi, true);
	for (int i = 0; i < selectedCards.size(); ++i)
	{
		ASSERT_EQ(selectedCards[i], result[i]);
	}    
}
int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}