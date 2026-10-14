package main

func maxSumOfSquares(num int, sum int) string {
	ans := make([]byte, 0)
	var dfs func(p int, pre int)
	f := false
	dfs = func(p int, pre int) {
		if f == true {
			return
		}
		if p == -1 {
			if pre == 0 {
				f = true
			}
			return
		}
		if (p+1)*9 < pre {
			return
		}
		for i := min(9, pre); i >= 0; i-- {
			if len(ans) == 0 && i == 0 {
				continue
			}
			ans = append(ans, byte('0'+i))
			dfs(p-1, pre-i)
			if f {
				return
			}
			ans = ans[:len(ans)-1]

		}
	}
	dfs(num-1, sum)
	return string(ans)
}
