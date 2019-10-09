
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

// 堆排序
// 向下过滤算法:时间复杂度O(log2(n))
func downfilter(seq []int, i, n int) {
	p := i 
	l := 2*p+1
	r := 2*p+2
	for ; l < n; {
		if r < n {
			var maxIdx int 
			if seq[l] > seq[r] {
				maxIdx = l
			} else {
				maxIdx = r
			}
			if seq[maxIdx] > seq[p] {
				seq[maxIdx], seq[p] = seq[p], seq[maxIdx]
				p = maxIdx
				l = 2*p+1
				r = 2*p+2
			} else {
				break
			}
		} else {
			if seq[l] > seq[p] {
				seq[l], seq[p] = seq[p], seq[l]
			}
			break
		}
	}
}
// 总体时间复杂度O(n*log2(n))
func HeapSort(seq []int) {
	n := len(seq)
	// 建堆(最大堆) 维护局部的堆序性
	for i := n/2; i >= 0; i-- {
		downfilter(seq, i, n)
	}
	// 开始排序 交换n-1次
	n--
	for ; n > 0; n-- {
		seq[0], seq[n] = seq[n], seq[0]
		downfilter(seq, 0, n)
	}
}

// 归并排序
// 合并的函数的时间复杂度为O(n) 空间复杂度为O(n)
func merge(seq []int, ll, lr, rl, rr int) {
	ls, le, rs, re := ll, lr, rl, rr
	tmpseq := []int{}
	for ; ls <= le && rs <= re; {
		if seq[ls] < seq[rs] {
			tmpseq = append(tmpseq, seq[ls])
			ls++
		} else {
			tmpseq = append(tmpseq, seq[rs])
			rs++
		}
	}
	for ls <= le {
		tmpseq = append(tmpseq, seq[ls])
		ls++
	} 
	for rs <= re {
		tmpseq = append(tmpseq, seq[rs])
		rs++
	}
	for i, v := range tmpseq {
		seq[ll+i] = v
	}
}
// 归并排序时间复杂度为O(nlog2(n))
func mergeSortHelp(seq []int, s, e int) {
	if s >= e {
		return
	}
	mid := (s+e)/2
	mergeSortHelp(seq, s, mid)
	mergeSortHelp(seq, mid+1, e)
	merge(seq, s, mid, mid+1, e)
}
func MergeSort(seq []int) {
	mergeSortHelp(seq, 0, len(seq)-1)
}

// 快速排序
// 一趟划分 确定一个值得最终位置
func partition(seq []int, s, e int) int {
	key := seq[s]
	for {
		for ; s < e && seq[e] >= key; e-- {
			
		}
		seq[s] = seq[e]
		for ; s < e && seq[s] <= key; s++ {

		}
		seq[e] = seq[s]
		if s >= e {
			break
		}
	}
	seq[s] = key 
	return s
}
func quickSortHelp(seq []int, s, e int) {
	if s >= e {
		return
	}
	pivot := partition(seq, s, e)
	quickSortHelp(seq, s, pivot)
	quickSortHelp(seq, pivot+1, e)
}
func QuickSort(seq []int) {
	quickSortHelp(seq, 0, len(seq)-1)
}