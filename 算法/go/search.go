
package baseSearch

func BinSearch(seq []int, key int) int {
	s := 0
	e := len(seq)-1
	for ; s <= e; {
		mid := (s+e)/2
		if seq[mid] == key {
			return mid
		} else if seq[mid] < key {
			s = mid+1
		} else {
			e = mid-1
		}
	}
	return -1
}