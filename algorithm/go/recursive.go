package recursive
// package main

import (
	"fmt"
	"math/rand"
	"math"
	"sort"
)

func factorial(n uint32) uint32 {
	if 0 == n {
		return 1
	}
	return n * factorial(n-1)
}

func fibonacci(n uint32) uint32 {
	if n <= 1 {
		return 1
	}
	return fibonacci(n-1) + fibonacci(n-2)
}
func swap(seq []byte, i, j int) {
	tmp := seq[i]
	seq[i] = seq[j]
	seq[j] = tmp
}
func perm(seq []byte, s, e int) {
	if s == e {
		for i := 0; i < e; i++ {
			fmt.Printf("%c", seq[i])
		}
		fmt.Println()
	} else {
		for i := s; i < e; i++ {
			swap(seq, s, i)
			perm(seq, s+1, e)
			swap(seq, s, i)
		}
	}
}

func move(a, b byte) {
	fmt.Printf("%c -> %c\n", a, b)
}

func hanoi(n int, a, b, c byte) {
	if n <= 0 {
		return
	}
	hanoi(n-1, a, c, b)
	move(a, b)
	hanoi(n-1, c, b, a)
}

func bs(seq []int, s, e, key int) int {
	l := s
	r := e - 1
	for l <= r {
		mid := (l + r) / 2
		if seq[mid] == key {
			return mid
		} else if seq[mid] < key {
			l = mid + 1
		} else {
			r = mid - 1
		}
	}
	return -1
}

func merge(seq []int, ll, lr, rl, rr int) {
	lefts, lefte, rights, righte := ll, lr, rl, rr 
	tmpseq := []int{}
	for lefts <= lefte && rights <= righte {
		if seq[lefts] < seq[rights] {
			tmpseq = append(tmpseq, seq[lefts])
			lefts++
		} else {
			tmpseq = append(tmpseq, seq[rights])
			rights++
		} 
	}
	for lefts <= lefte {
		tmpseq = append(tmpseq, seq[lefts])
		lefts++
	}
	for rights <= righte {
		tmpseq = append(tmpseq, seq[rights])
		rights++
	}
	for i := 0; i < len(tmpseq); i++ {
		seq[ll+i] = tmpseq[i]
	}
}

func mergeSort(seq []int, l, r int) {
	if l >= r {
		return 
	}
	mid := (l+r) / 2
	mergeSort(seq, l, mid)
	mergeSort(seq, mid+1, r)
	merge(seq, l, mid, mid+1, r)
}

func swapArrayValue(seq []int, i, j int) {
	tmp := seq[i]
	seq[i] = seq[j]
	seq[j] = tmp
}

func partition(seq []int, l, r int) int {
	key := seq[l]
	for {
		for ; l < r && key <= seq[r]; r-- {

		}	
		seq[l] = seq[r]
		for ; l < r && seq[l] <= key; l++ {

		}
		seq[r] = seq[l]
		if l >= r {
			break
		}
	}
	seq[l] = key
	return l
}

func quickSort(seq []int, l, r int) {
	if l >= r {
		return
	}
	pivot := partition(seq, l, r)
	quickSort(seq, l, pivot-1)
	quickSort(seq, pivot+1, r)
}

//	|		|		|
//  p       q       r
// 每趟划分确定第k大的元素是在q左边还是右边 递归调用
func linearSelect(seq []int, p, r, k int) int {
	if p == r {
		return seq[p]
	}
	// 基准值随机化
	hole := p + rand.Int() % (r-p+1)
	swapArrayValue(seq, hole, p)

	q := partition(seq, p, r)
	j := q - p + 1
	if k == j {
		return seq[q]
	} else if k < j {
		return linearSelect(seq, p, q, k)
	} else {
		return linearSelect(seq, q+1, r, k-j)
	}
}

// func main() {
// 	// seq := []byte("123")
// 	// perm(seq, 0, len(seq))
// 	// hanoi(3, 'a', 'b', 'c')
// 	// seq := []int{1, 3, 5, 7, 9}
// 	r := bs(seq, 0, len(seq), 5)
// 	fmt.Println(r)
// }

// closest pair
type Point2d struct {
	x float64
	y float64
}

type Point2ds []Point2d

func (p Point2ds) Len() int {
	return len(p)
}

func (p Point2ds) Swap(i, j int) {
	p[i], p[j] = p[j], p[i]
}

type SortByX struct {
	Point2ds
}

type SortByY struct {
	Point2ds
}

func (p SortByX) Less(i, j int) bool {
	return p.Point2ds[i].x < p.Point2ds[j].x
}

func (p SortByY) Less(i, j int) bool {
	return p.Point2ds[i].y < p.Point2ds[j].y
}

func dist(p1 *Point2d, p2 *Point2d) float64 {
	return math.Sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y))
}

// 数据规模足够小时 直接计算最接近的点对
func bruteForce(P []Point2d, n int) float64 {
	minFloat := math.MaxFloat64
	for i := 0; i < n; i++ {
		for j := i+1; j < n; j++ {
			currentDist := dist(&P[i], &P[j])
			if currentDist < minFloat {
				minFloat = currentDist
			}
		}
	}
	return minFloat
}

// y方向最接近点对
func stripClosest(strip []Point2d, size int, d float64) float64 {
	min := d 
	for i := 0; i < size; i++ {
		for j := i+1; j < size && strip[j].y - strip[j].y < min; j++ {
			currentDist := dist(&strip[i], &strip[j])
			if currentDist < min {
				min = currentDist
			}
		}
	}
	return min
}

func closestUtil(Px, Py []Point2d, n int) float64 {
	// 按照x分的足够小以后 直接计算
	if n <= 3 {
		return bruteForce(Px, n)
	}
	mid := n/2
	midPoint := Px[mid]

	// 点对分类 按照x坐标分成左右两部分
	var Pyl []Point2d
	var Pyr []Point2d
	for i := 0; i < n; i++ {
		if Py[i].x <= midPoint.x {
			Pyl = append(Pyl, Py[i])
		} else {
			Pyr = append(Pyr, Py[i])
		}
	}

	// 左半边最近接点对距离
	dl := closestUtil(Px[:mid], Pyl, mid)
	// 右半边最接近点对距离
	dr := closestUtil(Px[mid:], Pyr, n-mid)

	// 左右两边 最接近的点对的距离
	d := math.Min(dl, dr)
	// 收集距离中点小于 最接近点对的点
	var strip []Point2d
	for i := 0; i < n; i++ {
		if (math.Abs(Py[i].x - midPoint.x) < d) {
			strip = append(strip, Py[i])
		}
	}
	// 求y方向的最小距离
	return math.Min(d, stripClosest(strip, len(strip), d))
}

func closest(P []Point2d) float64 {
	var Px []Point2d
	var Py []Point2d
	for i := 0; i < len(P); i++ {
		Px = append(Px, P[i])
		Py = append(Py, P[i])
	}
	sort.Sort(SortByX{Px})
	sort.Sort(SortByY{Py})
	return closestUtil(Px, Py, len(P))
} 
