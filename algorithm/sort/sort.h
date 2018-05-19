#include <vector>

using namespace std;

template<typename T>
void Swap(T& t1, T& t2)
{
	T tmp(t1);
	t1 = t2;
	t2 = tmp;
}

// 冒泡排序
// n次 每次从序列种找出一个最小的元素下标，最后交换
// O(n^2)
// 交换语义
template<typename T>
void SortBubble(vector<T>& arr)
{
	int len = arr.size();
	for (int i = 0; i < len - 1; ++i)
	{
		int minIndex = i;
		for (int j = i + 1; j < len; ++j)
		{
			if (arr[minIndex] > arr[j])
			{
				minIndex = j;
			}
		}
		Swap<T>(arr[minIndex], arr[i]);
	}
}

// 插入排序
// 局部有序到整体有序
// O(n^2)
// 交换语义
template<typename T>
void SortInsert(vector<T>& arr)
{
	int len = arr.size();
	// 切分局部
	// 1
	// 1 2
	// 1 2 3
	// 1 2 3 ... n
	for (int i = 1; i < len; ++i)
	{
		// 使每一个局部序列有序
		for (int j = i; j >= 0; --j)
		{
			if (arr[j - 1] > arr[j])
			{
				Swap<T>(arr[j - 1], arr[j]);
			}
		}
	}
}

// 希尔排序(插入排序的优化)
template<typename T>
void SortShell(vector<T>& arr)
{
	int len = arr.size();
	for (int gap = len / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < len; i += gap)
		{
			for (int j = i; j >= 0; j -= gap)
			{
				if (j - gap >= 0 && arr[j - gap] > arr[j])
				{
					Swap<T>(arr[j - gap], arr[j]);
				}
			}
		}
	}
}

// 分区
// [start, end]
// 覆盖语义 非交换语义
// template<typename T>
// int Partition(vector<T>& arr, int start, int end)
// {
// 	// 常量 非常引用 非标签
// 	const T pivot = arr[start];
// 	while (start != end)
// 	{
// 		while (start != end && arr[end] > pivot)
// 		{
// 			--end;
// 		}
// 		if (start < end)
// 		{
//			// 覆盖
// 			arr[start] = arr[end];
// 			++start;
// 		}
// 		while (start != end && arr[start] < pivot)
// 		{
// 			++start;
// 		}
// 		if (start < end)
// 		{
//			// 覆盖
// 			arr[end] = arr[start];
// 			--end;
// 		}
// 	}
//  // 填补空位
// 	arr[start] = pivot;
// 	return start;
// }

// 交换语义
template<typename T>
int Partition(vector<T>& arr, int start, int end)
{
	const T pivot = arr[start];
	while (start != end)
	{
		// 从右边找一个比基准小的值
		while (start != end && arr[end] > pivot)
		{
			--end;
		}
		// 从左边找一个比基准大的值
		while (start != end && arr[start] < pivot)
		{
			++start;
		}
		if (start >= end)
		{
			break;
		}
		// 交换一大一小两个值
		Swap(arr[start], arr[end]);
	}
	return start;
}
// 快速排序
// [start, end]
// O(nlog(n))
template<typename T>
void DoSortQuick(vector<T>& arr, int start, int end)
{
	if (end > start)
	{
		// 分治
		int index = Partition(arr, start, end);
		// 左边递归
		DoSortQuick(arr, start, index - 1);
		// 右边递归
		DoSortQuick(arr, index + 1, end);
	}
}
template<typename T>
void SortQuick(vector<T>& arr)
{
	DoSortQuick(arr, 0, arr.size() - 1);
}
// 

// [start, end]
template<typename T>
void DoSortMerge(vector<T>& arr, T assist[], int start, int end)
{
	if (start >= end)
	{
		return;
	}
	int mid = (start + end) / 2;
	// int len = end - start, mid = (len >> 1) + start;
	int left1 = start;
	int right1 = mid;
	int left2 = mid + 1;
	int right2 = end;
	DoSortMerge(arr, assist, left1, right1);
	DoSortMerge(arr, assist, left2, right2);
	int index = start;
	while (left1 <= right1 && left2 <= right2)
	{
		assist[index++] = (arr[left1] < arr[left2]) ? arr[left1++] : arr[left2++];
	}
	while (left1 <= right1)
	{
		assist[index++] = arr[left1++];
	}
	while (left2 <= right2)
	{
		assist[index++] = arr[left2++];
	}
	for (index = start; index <= end; ++index)
	{
		arr[index] = assist[index];
	}
}

// 归并排序
template<typename T>
void SortMerge(vector<T>& arr)
{
	T* assist = new T[arr.size()];
	DoSortMerge(arr, assist, 0, arr.size() - 1);
}

template<typename T>
void DownFilter(vector<T>& arr, int len, int index)
{
	int parent = index;
	int left = 2 * parent + 1;
	int right = 2 * parent + 2;
	while (left < len)
	{
		// 存在左右子节点
		if (right < len)
		{
			int max = arr[left] > arr[right] ? left : right;
			// 子节点中的最大值大于父节点
			if (arr[max] > arr[parent])
			{
				Swap(arr[max], arr[parent]);
				parent = max;
				left = 2 * parent + 1;
				right = 2 * parent + 2;
			}
			// 父节点大于子节点
			else
			{
				break;
			}	
		}
		// 只存在左子节点
		else
		{
			if (arr[left] > arr[parent])
			{
				Swap(arr[left], arr[parent]);
			}
			break;
		}
	}
}

// 堆排序
template<typename T>
void SortHeap(vector<T>& arr)
{
	int len =  arr.size();
	// 建堆
	for (int i = len / 2 - 1; i >= 0; --i)
	{
		DownFilter(arr, len, i);
	}
	// 进行n-1次筛选
	for (int index = len - 1; index >= 0; --index)
	{
		// 交换堆顶最大元素到最后的位置(已有序集合递增)
		Swap(arr[index], arr[0]);
		// 排除堆中的最大值(待排序集合递减),确定当前堆顶元素的最终位置
		DownFilter(arr, index, 0);
	}
}