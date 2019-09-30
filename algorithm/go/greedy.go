
package main 

import (
	"fmt"
	"bytes"
	"strconv"
	"sort"
)

// Q: 如果我们有面值为1元、3元和5元的硬币若干枚，如何用最少的硬币凑够11元？

func minCoinReachTargetMoney(targetMoney int) int {
	coins := []int{1, 3, 5}
	count := 0
	minCoin := coins[0]
	currentMoney := targetMoney
	for currentMoney >= minCoin {
		for i := len(coins)-1; i >= 0; i-- {
			if currentMoney >= coins[i] {
				currentMoney -= coins[i]
				count++
				break
			}
		}
	}
	if currentMoney == 0 {
		return count
	}
	return 0
}

func TestMinCoinReachTargetMoney() {
	num := minCoinReachTargetMoney(11)
	fmt.Println(num)
}


func min(x, y int) int {
	if x < y {
		return x
	}
	return y
}

// 分别有1,5,10,50,100元 分别有5,2,2,3,5张纸币 问若要支付k元 则需要多少张纸币
func currencySelection(k int) int {
	moneys := []int{1, 5, 10, 50, 100}
	counts := []int{5, 2, 2,   3,   5}
	num := 0
	for i := len(moneys)-1; i >= 0 && k >= moneys[0]; i-- {
		amount := min(k/moneys[i], counts[i])
		k = k - amount * moneys[i]
		num += amount

	}
	if k == 0 {
		return num
	}
	return 0
}

func TestCurrencySelection() {
	num := currencySelection(520)
	fmt.Println(num)
}

// count活动数量
// start end 活动开始时间列表和活动结束时间列表
// selected 最终被选中的活动编号
func activityPlanning(count int, start, end []int) []int {
	selected := make([]int, 0)
	i := 1
	j := 0
	for i <= count {
		// 当前活动开始时间大于上一个选中活动的结束时间
		if start[i] >= end[j] {
			selected = append(selected, i)
			j = i
			i++
		} else {
			i++
		}
	}
	return selected
}

func TestActivityPlanning() {
	// 活动已按照时间有序排列
	// 0位置座位哨兵值
	s := [12]int{0,1,3,0,5,3,5,6,8,8,2,12}
	e := [12]int{0,4,5,6,7,8,9,11,12,13,14}
	n := 11

	selected := activityPlanning(n, s[:], e[:])
	for _, v := range selected {
		fmt.Print(v, " ")
	}
	fmt.Println()
}

// 给定长度为N的字符串S 要构造一个字符串T 起初T为空字符串 随后反复进行一下操作 直到S为空串
// 从S的头部删除一个字符 加到T的尾部
// 从S的尾部删除一个字符 加到T的尾部
// 目标要构造一个字典序尽可能小的字符串T
// ACDBCB	=>		ABCBCD
func bestCowLine(seq string) string {
	var buff bytes.Buffer
	s := 0
	e := len(seq)-1
	for s <= e {
		flag := false
		// 从当前[s-e]范围内 找最小的字符
		for i := 0; s+i < e; i++ {
			if seq[s+i] < seq[e-i] {
				flag = true 
				break
			} else if seq[e-i] < seq[s+i] {
				flag = false
				break
			} else {
				// 首尾相等
			}
		}
		// 每一步贪心
		// 头部更小
		if flag {
			buff.WriteByte(seq[s])
			s++
		// 尾部更小
		} else {
			buff.WriteByte(seq[e])
			e--
		}
	}
	return buff.String()
}

func TestBestCowLine() {
	seq := "ACDBCB"
	r := bestCowLine(seq)
	fmt.Println(r)
}

// 均分纸牌
// 均分纸牌问题：有 N 堆纸牌，编号分别为 1，2，…, N。每堆上有若干张，但纸牌总数必为 N 的倍数。可以在任一堆上取若于张纸牌，然后移动。

// 　　移牌规则为：在编号为 1 堆上取的纸牌，只能移到编号为 2 的堆上；在编号为 N 的堆上取的纸牌，只能移到编号为 N-1 的堆上；其他堆上取的纸牌，可以移到相邻左边或右边的堆上。

// 　　现在要求找出一种移动方法，用最少的移动次数使每堆上纸牌数都一样多。

// 　　例如 N=4，4 堆纸牌数分别为：

// 　　①　9　②　8　③　17　④　6

// 　　移动3次可达到目的：

// 　　从 ③ 取 4 张牌放到 ④ （9 8 13 10） -> 从 ③ 取 3 张牌放到 ②（9 11 10 10）-> 从 ② 取 1 张牌放到①（10 10 10 10）。
func dividePoker(pokers []int) int {
	n := len(pokers)
	total := 0
	for _, v := range pokers {
		total += v
	}
	avg := total / n 
	count := 0
	for i := 0; i < n; i++ {
		if pokers[i] != avg {
			pokers[i+1] = pokers[i+1] - (avg - pokers[i])	// 第i堆向第i+1堆要牌 不足够平均值 此过程继续下去 核心在于牌堆数可暂时未负数
			pokers[i] = avg
			count++
		}
	}
	return count
}

func TestDividePoker() {
	pokers := []int{9, 8, 17, 6}
	count := dividePoker(pokers)
	fmt.Println(count)
}


func maxInteger(seq []int) string {
	n := len(seq)
	// 排序
	for i := 0; i < n-1; i++ {
		for j := i+1; j < n; j++ {
			// 排序的比较方法 比较两数相加的字符串序
			ijstr := strconv.Itoa(seq[i]) + strconv.Itoa(seq[j])
			jistr := strconv.Itoa(seq[j]) + strconv.Itoa(seq[i])
			if ijstr < jistr {
				tmp := seq[i]
				seq[i] = seq[j]
				seq[j] = tmp
			}
		}
	}
	r := ""
	for _, v := range seq {
		r += strconv.Itoa(v)
	}
	return r
}

func TestMaxInteger() {
	seq := []int{12, 121}
	r := maxInteger(seq)
	fmt.Println(r)
}

// 有一批集装箱要装上一艘载重量为c的轮船 其中集装箱i的重量为Wi 最优装载问题要求确定在装载体积不受限制的情况下 将尽可能多的集装箱装上轮船
func bestLoad(c int, w []int) []int {
	r := make([]int, 0)
	sort.Sort(sort.IntSlice(w))
	for i := 0; i < len(w); i++ {
		if c < w[i] {
			break
		}
		c -= w[i]
		r = append(r, w[i])
	}
	return r
}

func TestBestLoad() {
	c := 30
	w := []int{2, 3, 4, 5, 7, 10, 11, 14}
	r := bestLoad(c, w)
	for _, v := range r {
		fmt.Print(v, " ")
	}
	fmt.Println()
} 

// 背包问题 - 贪心算法

// 旅行商问题 - 贪心算法

// 哈夫曼编码

// 最小生成树

// 最短路径


func main()  {
	// TestMinCoinReachTargetMoney()
	// TestCurrencySelection()
	// TestActivityPlanning()
	// TestBestCowLine()
	// TestDividePoker()
	// TestMaxInteger()
	// TestBestLoad()
}

