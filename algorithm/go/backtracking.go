
package main 

import (
	"fmt"
	"math"
	// "bytes"
)

func show(n int, q []int) {
	for i := 1; i <= n; i++ {
		fmt.Printf("(%d,%d)", i, q[i])
	}
	fmt.Println("\n---")
	for i := 1; i <= n; i++ {
		for j := 1; j <= n; j++ {
			if q[i] != j {
				fmt.Print("X ")
			} else {
				fmt.Print("Q ")
			}
		}
		fmt.Println()
	}
	fmt.Println()
}

func canPlace(curRow, curCol int, q []int) bool {
	r := true
	// 当前行之前的行
	for iRow := 1; iRow < curRow; iRow++ {
		// 同列 或 对角线
		if q[iRow] == curCol || math.Abs(float64(iRow-curRow)) == math.Abs(float64(q[iRow] - curCol)) {
			r = false
			break
		}
	}
	return r
}

func place(kRow, n int, q []int) {
	// 所有行试探完成
	if kRow > n {
		show(n, q)
	} else {
		// 循环试探第k行的每一列
		for col := 1; col <= n; col++ {
			// 约束条件
			if (canPlace(kRow, col, q)) {
				q[kRow] = col	// 选出第k行可放的位置
				place(kRow+1, n, q)	// 递归试探第k+1行
			}
		}
	}
}

func nQueen(n int) {
	q := make([]int, n+1)
	// q表示n个皇后的最终位置 index表示行 value表示列
	// 用一维的数组代替[][]int二维数组表示的皇后的最终位置
	// 0索引位置不用 皇后计数范围为[1,n]
	place(1, n, q)
}

// https://www.jianshu.com/p/bb123944d3e5
func nQueenLoop(n int) {
	q := make([]int, n+1)
	i, j := 1, 1	// 从第一行第一列开始
	for i <= n {
		// 判断一行
		for j <= n {	// 循环试探第i行的每一列
			if canPlace(i, j, q) {	// 第i行j列 可以放置
				q[i] = j	// i行j列放皇后
				j = 1	// 从第一列从新开始
				break
			} else {
				j++		// i行j列不能放 看i行j+1列
			}
		}
		// 一行判断完成的结果
		// 这一行没有找到能放的 说明上一行选择的皇后位置不合适
		if q[i] == 0 {	// 第i行的n个列中没找到可以放置皇后的位置
			if i == 1 {	// 已经回溯到第一行 依然找不到 程序终止
				break
			} else {
				i-- 
				j = q[i] + 1	// 把上一个皇后的位置往右移一列 j回退
				q[i] = 0	// 清除上一行皇后的位置 重新探测
				continue
			}
		}
		// 由于上面的判断 一行找完的结果如果是没找到 要么后退一行要么结束
		// 此处必然是找到可以放的位置
		// 当前行是最后一行 说明一个可行的解已经找到
		if i == n {		// 最后一行找到可以放皇后的位置
			show(n, q)
			j = q[i] + 1	// 从最后一个放皇后的位置的下一列继续 j回退 
			q[i] = 0	// 清除最后一行的皇后位置
			continue
		}
		i++	// 继续下一行皇后位置
	}
}


func TestNQueen() {
	// nQueen(8)
	nQueenLoop(4)
}

func combination(digits string, curIdex int, s string, digitletterseq []string, r *[]string) {
	if curIdex >= len(digits) {
		*r = append(*r, s)
	} else {
		letters := digitletterseq[digits[curIdex]-'0']
		// 迭代某数字代表的字符
		for _, c := range letters {
			// 递归下一个数字
			combination(digits, curIdex+1, s + string(c), digitletterseq, r)
		}
	}
}
func letterCombinations(digits string) []string {
	r := make([]string, 0)
	if len(digits) == 0 {
		return r
	}
	digitletterseq := []string{"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"}
	combination(digits, 0, "", digitletterseq, &r)
	return r
}

func TestLetterCombinations() {
	r := letterCombinations("23")
	for _, v := range r {
		fmt.Print(v, " ")
	}
	fmt.Println()
}

func permuteRecursive(s, e int, nums []int, r *[][]int) {
	if s >= e {
		t := make([]int, len(nums))
		copy(t, nums)
		*r = append(*r, t)
	} else {
		for i := s; i < e; i++ {
			nums[i], nums[s] = nums[s], nums[i]
			permuteRecursive(s+1, e, nums, r)
			nums[i], nums[s] = nums[s], nums[i]
		}
	}
}

func permute(nums []int) [][]int {
	var r [][]int
	permuteRecursive(0, len(nums), nums, &r)
	return r
}

func TestPermute() {
	seq := []int{1, 2, 3}
	r := permute(seq)
	for _, seqs := range r {
		for _, v := range seqs {
			fmt.Print(v, " ")
		}
		fmt.Println()
	}
}

func permuteUniqueRecursive(s, e int, nums []int, r *[][]int) {
	if s >= e {
		t := make([]int, len(nums))
		copy(t, nums)
		*r = append(*r, t)
	} else {
		for i := s; i < e; i++ {
			if i != s && nums[i] == nums[s] {
				continue
			}
			nums[s], nums[i] = nums[i], nums[s]
			permuteUniqueRecursive(s+1, e, nums, r)
			nums[i], nums[s] = nums[s], nums[i]
		}
	}
}
func permuteUnique(nums []int) [][]int {
	var r [][]int
	permuteUniqueRecursive(0, len(nums), nums, &r)
	return r
}
func TestPermuteUnique() {
	seq := []int{1, 1, 2, 2}
	r := permuteUnique(seq)
	for _, seqs := range r {
		for _, v := range seqs {
			fmt.Print(v, " ")
		}
		fmt.Println()
	}
}
func main() {
	// TestNQueen()
	// TestLetterCombinations()
	// TestPermute()
	TestPermuteUnique()
}