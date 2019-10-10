#include <iostream>
#include <gtest/gtest.h>
#include "sort.h"

using testing::Test;
using namespace std;

class TestSort : public Test
{
    void SetUp()
    {
        cout << "TestSort::SetUp" << endl;
    }
    void TearDown()
    {
        cout << "TestSort::TearDown" << endl;
    }
};

// 选择排序单元测试
TEST_F(TestSort, SortSelectTest)
{
	int arr[] = { 1, 3, 2, 4, 5, 7, 6, 8, 9, 0 };
    int result[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len = sizeof(arr) / sizeof(arr[0]);
	std::vector<int> vec(arr, arr + len);
	SortSelect(vec);
	for (int i = 0; i < vec.size(); ++i)
	{
		ASSERT_EQ(vec[i], result[i]);
	}    
}

// 插入排序单元测试
TEST_F(TestSort, SortInsertTest)
{
	int arr[] = { 1, 3, 2, 4, 5, 7, 6, 8, 9, 0 };
    int result[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len = sizeof(arr) / sizeof(arr[0]);
	std::vector<int> vec(arr, arr + len);
	SortInsert(vec);
	for (int i = 0; i < vec.size(); ++i)
	{
		ASSERT_EQ(vec[i], result[i]);
	}    
}

// 快速排序单元测试
TEST_F(TestSort, SortQuickTest)
{
	int arr[] = { 1, 3, 2, 4, 5, 7, 6, 8, 9, 0 };
    int result[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len = sizeof(arr) / sizeof(arr[0]);
	std::vector<int> vec(arr, arr + len);
	SortQuick(vec);
	for (int i = 0; i < vec.size(); ++i)
	{
		ASSERT_EQ(vec[i], result[i]);
	}    
}

// 希尔排序单元测试
TEST_F(TestSort, SortShellTest)
{
	int arr[] = { 1, 3, 2, 4, 5, 7, 6, 8, 9, 0 };
    int result[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len = sizeof(arr) / sizeof(arr[0]);
	std::vector<int> vec(arr, arr + len);
	SortShell(vec);
	for (int i = 0; i < vec.size(); ++i)
	{
		ASSERT_EQ(vec[i], result[i]);
	}    
}

// 归并排序单元测试
TEST_F(TestSort, SortMergeTest)
{
	int arr[] = { 1, 3, 2, 4, 5, 7, 6, 8, 9, 0 };
    int result[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len = sizeof(arr) / sizeof(arr[0]);
	std::vector<int> vec(arr, arr + len);
	SortMerge(vec);
	for (int i = 0; i < vec.size(); ++i)
	{
		ASSERT_EQ(vec[i], result[i]);
	}    
}

// 堆排序单元测试
TEST_F(TestSort, SortHeapTest)
{
	int arr[] = { 1, 3, 2, 4, 5, 7, 6, 8, 9, 0 };
	int result[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int len = sizeof(arr) / sizeof(arr[0]);
	std::vector<int> vec(arr, arr + len);
	SortHeap(vec);
	for (int i = 0; i < vec.size(); ++i)
	{
		ASSERT_EQ(vec[i], result[i]);
	}    
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}