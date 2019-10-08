/*
 * @lc app=leetcode.cn id=17 lang=golang
 *
 * [17] 电话号码的字母组合
 */
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

