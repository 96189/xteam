
package baseSort

// 冒泡排序
// 时间复杂度O(n*n)
// 空间复杂度O(1)
// 稳定性:稳定
// 最好情况下(序列本身有序)时间复杂度:O(n)
func BubbleSort(seq []int) {
	n := len(seq)
	for i := 0; i < n; i++ {
		swapCount := 0
		for j := n-1; j > i; j-- {
			// 两两交换 一次循环交换多次
			if seq[j] < seq[j-1] {
				seq[j], seq[j-1] = seq[j-1], seq[j]
				swapCount++
			}
		}
		if swapCount == 0 {
			break
		}
	}
}

// 选择排序
// 时间复杂度O(n*n)
// 空间复杂度O(1)
// 稳定性:不稳定
// 没有最好的情况 算法复杂度始终为O(n*n)
func SelectSort(seq	[]int) {
	n := len(seq)
	for i := 0; i < n; i++ {
		minIdx := i
		for j := i+1; j < n; j++ {
			if seq[j] < seq[minIdx] {
				minIdx = j
			}
		}
		// 选择极值交换 一次循环最多只交换一次
		if minIdx != i {
			seq[i], seq[minIdx] = seq[minIdx], seq[i]
		}
	}
}

// 插入排序
// 时间复杂度O(n*n)
// 空间复杂度O(1)
// 稳定性:稳定
// 最好的情况下(序列本身有序)时间复杂度:O(n)
func InsertSort(seq []int) {
	n := len(seq)
	// uIdx无序起始位置 nextIdx有序的下一个位置
	for uIdx := 1; uIdx < n; uIdx++ {
		for nextIdx := uIdx; nextIdx > 0; nextIdx-- {
			if seq[nextIdx-1] > seq[nextIdx] {
				seq[nextIdx-1], seq[nextIdx] = seq[nextIdx], seq[nextIdx-1]
			} else {
				break
			}
		}
	}
}