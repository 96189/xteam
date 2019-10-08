
package baseSort

func bubbleSort(seq	[]int) {
	n := len(seq)
	for i := 0; i < n; i++ {
		minIdx := i
		for j := i+1; j < n; j++ {
			if seq[j] < seq[minIdx] {
				minIdx = j
			}
		}
		if minIdx != i {
			seq[i], seq[minIdx] = seq[minIdx], seq[i]
		}
	}
}