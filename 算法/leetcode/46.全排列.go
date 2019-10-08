/*
 * @lc app=leetcode.cn id=46 lang=golang
 *
 * [46] 全排列
 */
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
