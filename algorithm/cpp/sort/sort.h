#include <vector>
#include <list>
using namespace std;

template <typename T>
void Swap(T &t1, T &t2)
{
	T tmp(t1);
	t1 = t2;
	t2 = tmp;
}

// 冒泡排序
// n次 每次从序列种找出一个最小的元素下标，最后交换
// O(n^2)
// 交换语义
template <typename T>
void SortBubble(vector<T> &arr)
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
template <typename T>
void SortInsert(vector<T> &arr)
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
template <typename T>
void SortShell(vector<T> &arr)
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
template <typename T>
int Partition(vector<T> &arr, int start, int end)
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
template <typename T>
void DoSortQuick(vector<T> &arr, int start, int end)
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
template <typename T>
void SortQuick(vector<T> &arr)
{
	DoSortQuick(arr, 0, arr.size() - 1);
}
//

// [start, end]
template <typename T>
void DoSortMerge(vector<T> &arr, T assist[], int start, int end)
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
template <typename T>
void SortMerge(vector<T> &arr)
{
	T *assist = new T[arr.size()];
	DoSortMerge(arr, assist, 0, arr.size() - 1);
}

template <typename T>
void DownFilter(vector<T> &arr, int len, int index)
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
template <typename T>
void SortHeap(vector<T> &arr)
{
	int len = arr.size();
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

// O(n)
int MaxDigits(int arr[], const int len, const int radix)
{
	int max = arr[0];
	for (int i = 0; i < len; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	int digits = 0;
	do
	{
		digits++;
		max = max / radix;
	} while (max);
	return digits;
}

// 计数排序
int MaxValue(int arr[], int len)
{
	int max = arr[0];
	for (int i = 0; i < len; ++i)
	{
		if (arr[i] > max)
		{
			max = arr[i];
		}
	}
	return max;
}
// 计数排序
// arr的[0]元素不适用,加入只有一个元素,小于等于该元素的个数至少为1,
// 该个数作为数组下标,因此不能使用0下标
// 测试用例int arr[] = { 0, 37, 26, 45, 19, 23, 17, 5, 33, 18 };
// int len = sizeof(arr) / sizeof(arr[0]);
// CountingSort(arr, len);
void CountingSort(int arr[], int len)
{
	int max = MaxValue(arr, len);

	int *assist = new int[len + 1];
	// key 数值
	// value 数值次数
	// 额外空间过大
	int *count = new int[max + 1];

	// 扫描arr建立count的key value表
	// 分组 一个元素一组
	for (int i = 1; i <= len; ++i)
	{
		count[arr[i]]++;
	}
	// O(n)时间复杂度过大
	// 假设有[m,n]共p个元素 n >> n
	// 线性遍历成本过大,辅助空间也过大
	// count[elem]表示不大于elem的元素的个数
	// 该个数同样表示elem之前数组中填充元素的个数
	// 该个数可以作为最终有序数组的下标
	// 统计
	for (int elem = 2; elem <= max; ++elem)
	{
		count[elem] = count[elem] + count[elem - 1];
	}
	// count[arr[i]]元素次数可作为下标
	// 根据统计的值保存结果
	// 倒序的遍历arr保证了排序算法的稳定性,
	// 相同数字在序列中的最后一个保证出现在assist中相对最后的位置
	for (int i = len; i >= 1; --i)
	{
		assist[count[arr[i]]] = arr[i];
		count[arr[i]]--;
	}
	for (int i = 1; i <= len; ++i)
	{
		arr[i] = assist[i];
	}
	delete[] assist;
	delete[] count;
}
// 桶排序的一种特殊情况
void SpecialBucketSort(int arr[], int len)
{
	int max = MaxValue(arr, len);

	int *assist = new int[len + 1];
	// key 数值
	// value 数值次数
	// 额外空间过大
	int *count = new int[max + 1];

	// 扫描arr建立count的key value表
	// 分组 一个元素一组
	for (int i = 1; i <= len; ++i)
	{
		count[arr[i]]++;
	}
	int elem;
	int start;
	int index;
	// n组
	for (elem = 1, start = 1; elem <= max; elem++, start = index)
	{
		// 一组
		for (index = start; index < start + count[elem]; ++index)
		{
			assist[index] = elem;
		}
	}
	for (int i = 1; i <= len; ++i)
	{
		arr[i] = assist[i];
	}
	delete[] assist;
	delete[] count;
}

// 桶排序
void BucketSort(int arr[], int len)
{
	const int bucket_num = 10;
	// define bucket
	std::vector<std::list<int> *> bucket(bucket_num, NULL);
	// hash到bucket_num个桶中
	for (int i = 0; i < len; ++i)
	{
		int index = arr[i] % bucket_num;
		std::list<int> *l = bucket.at(index);
		if (l == NULL)
		{
			l = new std::list<int>;
			bucket[index] = l;
		}
		l->push_front(arr[i]);
	}

	// 取第一个桶
	std::list<int> *l = bucket[0];
	if (l == NULL)
	{
		l = new std::list<int>;
		bucket[0] = l;
	}
	l->sort();
	// 合并其他桶的数据到第一个桶
	for (int i = 1; i < bucket_num; ++i)
	{
		std::list<int> *ll = bucket.at(i);
		if (ll)
		{
			ll->sort();
			l->merge(*ll);
		}
	}
	// 拷贝已有序数据到原数据
	int index = 0;
	for (std::list<int>::iterator it = l->begin(); it != l->end(); ++it)
	{
		arr[index++] = *it;
	}
	// 清理额外内存
	for (std::vector<std::list<int> *>::iterator it = bucket.begin(); it != bucket.end(); ++it)
	{
		if (*it)
		{
			(*it)->clear();
			delete (*it);
		}
	}
	bucket.clear();
}

// 基数排序
// radix进制 2 8 10 16进制
void RadixSort(int arr[], const int len, const int radix)
{
	// assert(10 == radix);
	int maxDigits = MaxDigits(arr, len, radix);

	// 辅助数组,按照顺序存储各分组的结果
	int *assist = new int[len];
	// 记录当前被分组到当前位置的数据个数
	// 记录小于当前分组中最大值的数据个数
	int *count = new int[radix];

	int digit = 1;
	int index = 0;
	// maxDigits times
	for (int i = 0; i < maxDigits; ++i)
	{
		// init or clear count array
		for (int j = 0; j < radix; ++j)
		{
			count[j] = 0;
		}
		// hash分组 n个元素一组
		for (int j = 0; j < len; ++j)
		{
			index = (arr[j] / digit) % radix;
			count[index]++;
		}
		// count 统计小于等于count[k]中最大数的个数
		for (int k = 1; k < radix; ++k)
		{
			count[k] = count[k] + count[k - 1];
		}
		// 根据count数组将原数据arr映射到assist中
		// 相当于一趟排序过程
		// 倒序扫描保证排序算法的稳定性
		// 相同的数组保证最后一个出现在assist中相对靠后的位置
		for (int m = len - 1; m >= 0; --m)
		{
			index = (arr[m] / digit) % radix;
			assist[count[index] - 1] = arr[m];
			count[index]--;
		}
		// 局部有序数据拷贝到原数据中
		for (int j = 0; j < len; ++j)
		{
			arr[j] = assist[j];
		}
		//
		digit = digit * radix;
	}
	delete[] assist;
	delete[] count;
}
