
package main

import (
	"fmt"
	//"math"
	"time"
	"math/rand"
)

// 斐波那契数列	1、1、2、3、5、8、13、21、34、……
// F(1)=1，F(2)=1, F(n)=F(n-1)+F(n-2)（n>=3，n∈N*）
func fibonacii(n int) int64 {
	// 保存计算结果的表
	dp := make([]int64, n+1)

	// 初始化计算表
	dp[1] = 1
	dp[2] = 1

	// 自底向上计算
	for number := 3; number <= n; number++ {
		dp[number] = dp[number-1] + dp[number-2]	// 递推式 中间计算过程查表
	}

	return dp[n]
}


// Q: 如果我们有面值为1元、3元和5元的硬币若干枚，如何用最少的硬币凑够11元？
// money(0) = 0										->	0
// money(1) = 1										->	1
// money(2) = 1+1									->	2
// money(3) = 1 	1+1+1							->	1	3
// money(4) = 1+3	1+1+1+1							->	2	4
// money(5) = 5     3+1+1		1+1+1+1+1			->	1	3	5
// money(6) = 5+1	3+3			1+1+1+1+1+1			->	2   6		

// f(goalMoney) = f(goalMoney-coin) + 1
// coin为 1 3 5中某个面值的硬币
func minCoinReachTargetMoney(targetMoney int) int {
	coins := [3]int{1, 2, 5}
	dp := make([]int, targetMoney+1)
	// 0表示无解 money小于最小的硬币无法凑够
	for money := 0; money < coins[0]; money++ {
		dp[money] = 0	// 保存计算结果的表
	}
	// 自底向上计算
	for goalMoney := coins[0]; goalMoney <= targetMoney; goalMoney++ {
		// coins升序排列 最后一次计算的为可用的最大的coin
		for _, coin := range coins {
			// 参与计算的coin 最大不能大于goalMoney
			if coin <= goalMoney {
				dp[goalMoney] = dp[goalMoney - coin] + 1	// 递推式 中间计算过程查表
			}
		}
	}
	return dp[targetMoney]
}

// 数组最大不连续递增子序列 最长非降序子序列

func max(x, y int) int {
	if (x >= y) {
		return x
	}
	return y
}

func longestIncreaseSubsequence(seq []int) int {
	n := len(seq)
	// 保存计算结果的表
	dp := make([]int, n+1)

	maxSeqLen := 0
	for index, value := range seq {
		// 当前位置 非降序子序列长度最少为1
		dp[index] = 1
		for j := 0; j < index; j++ {
			// 非降序
			if seq[j] <= value {
				dp[index] = max(dp[index], dp[j]+1)		// 递推式 中间计算结果查表
			}
		}
		maxSeqLen = max(maxSeqLen, dp[index])
	}
	return maxSeqLen
}


// 数组最大连续子序列和
func maxLinkSeqSum(seq []int) int {
	// 保存计算结果 设定初始值
	curSum := seq[0]

	maxSum := curSum
	for i := 1; i < len(seq); i++ {
		// 递推式
		curSum = max((curSum + seq[i]), seq[i])		
		if curSum > maxSum {
			maxSum = curSum
		}
	}
	return maxSum
}

// 最长公共子序列
func LCS(x, y string) {
	lenX := len(x)
	lenY := len(y)

	// 保存计算结果的表 递推计算依赖前一次的值 
	// 从前往后计算时 第一个字符之前的值设置为0
	// 从后往前计算时 最后一个字符之前的值设置为0 
	dp := make([][]int, (lenX+1), (lenX+1))
	for row := 0; row < lenX+1; row++ {
		rowArray := make([]int, (lenY+1), (lenY+1))
		dp[row] = rowArray
	}

	// 自底向上递推计算	dp[i][j]表示当前位置计算出公共子序列的长度 
	// i标记x j标记y dp[i+1][j] dp[i][j+1]两个值的相对大小判断当前位置最长公共子序列是在x中还是y中选择
	
	// 假设第一个字符是相当的 从前向后计算
	for i := 1; i < lenX; i++ {
		for j := 1; j < lenY; j++ {
			if x[i] == y[j] {
				dp[i][j] = dp[i-1][j-1] + 1
			} else {
				dp[i][j] = max(dp[i-1][j], dp[i][j-1])
			}
		}
	}
	// 假设最后一个字符是相等的 从后向前计算
	// for i := lenX-1; i >= 0; i-- {
	// 	for j := lenY-1; j >= 0; j-- {
	// 		if x[i] == y[j] {
	// 			dp[i][j] = dp[i+1][j+1] + 1
	// 		} else {
	// 			dp[i][j] = max(dp[i+1][j], dp[i][j+1])
	// 		}
	// 	}
	// }

	fmt.Println("x:", x)
	fmt.Println("y:", y)
	fmt.Print("x & y lcs:")

	// 遍历获取公共子序列
	i, j := 0, 0
	for i < lenX && j < lenY {
		if x[i] == y[j] {
			fmt.Printf("%c", x[i])
			i++
			j++
		} else if dp[i+1][j] >= dp[i][j+1] {
			i++
		} else {
			j++
		}
	}
	fmt.Println()
}


func GetRandomString(lens int) string {
	str := "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	bytes := []byte(str)
	result := []byte{}
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	for i := 0; i < lens; i++ {
		result = append(result, bytes[r.Intn(len(bytes))])
	}
	return string(result)
}

func TestLCS() {
	x := "abcbdab"//"acbd"//"conservatives"//"abcfbc"//GetRandomString(18)
	y := "bdcaba"//"abedc"//"breather"//"abfcab"//GetRandomString(18)
	LCS(x, y)
}

// w 价值(权重)
// v 体积
// c 背包容量
func KnapSack01(w, v []int, c int) int {
	// 断言 len(w) == len(v)
	n := len(w)
	// 定义保存计算结果的表
	// 下标表示背包容量 值表示该容量下的最大价值
	dp := make([]int, c+1, c+1)

	// 递推计算
	for i := 0; i < n; i++ {
		// 计算所有可行的背包容量的情况 判断i物品放还是不放
		for surplus := c; surplus >= v[i]; surplus-- {
			dp[surplus] = max(dp[surplus], w[i] + dp[surplus - v[i]])	// 当前背包中物品体积的价值  取决于当前物品是否放入 不放的价值  与放入后当前物品的价值和剩余体积的价值
		}
	}
	for i, v := range dp {
		fmt.Printf("%d:%d ", i, v)
	}
	fmt.Println()
	return dp[c]
}

func TestKnapSack01() {
	v := []int{1, 2, 3}
	w := []int{12, 10, 6}
	c := 5
	totalw := KnapSack01(w, v, c)
	fmt.Println(totalw)
}

func main() {
	// num := minCoinReachTargetMoney(11)
	// fmt.Println(num)
	
	// 46368
	// fib24 := fibonacii(24)
	// fmt.Println(fib24)

	// seq := []int{2, 2, 5, 1, 1, 3, 4, 8, 6, 7}
	// maxSeqLen := longestIncreaseSubsequence(seq)
	// fmt.Println(maxSeqLen)

	// seq := []int{5, 3, -8, 6, 1}
	// maxSum := maxLinkSeqSum(seq)
	// fmt.Println(maxSum)
	// TestLCS()
	TestKnapSack01()
}