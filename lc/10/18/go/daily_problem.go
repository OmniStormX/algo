// https://leetcode.cn/problems/number-of-ways-to-reorder-array-to-get-same-bst
package main

import "fmt"

func numOfWays(nums []int) int {
	// C_n^i
	ans := 0
	M := int64(1000000007)

	cur := 0
	n := len(nums)
	ls := make([]int, n+9)
	rs := make([]int, n+9)
	val := make([]int, n+9)
	cnt := make([]int, n+9)

	var insert func(p, x int) int
	insert = func(p, x int) int {
		if val[p] == 0 {
			cur++
			cnt[cur] = 1
			val[cur] = x
			return cur
		}
		if x < val[p] {
			ls[p] = insert(ls[p], x)
		} else {
			rs[p] = insert(rs[p], x)
		}
		cnt[p]++
		return p
	}
	for i := 0; i < n; i++ {
		insert(1, nums[i])
	}
	fac := make([]int, n+1)

	fac[0] = 1
	for i := 1; i <= n; i++ {
		fac[i] = int(int64(fac[i-1]) * int64(i) % int64(M))
	}

	qpow := func(x, y int) int {
		ans := 1
		for y > 0 {
			if y&1 == 1 {
				ans = int(int64(ans) * int64(x) % int64(M))
			}
			x = int(int64(x) * int64(x) % int64(M))
			y >>= 1
		}
		return ans
	}

	C := func(n, m int) int {
		// n! / ((n - m)!m!)
		return int(int64(fac[n]) * int64(qpow(int(int64(fac[m])*int64(fac[n-m])%int64(M)), int(M-2))) % M)
	}

	var dfs func(p int) int
	dfs = func(p int) int {
		ret := 1
		if ls[p] > 0 {
			retl := dfs(ls[p])
			ret = int(int64(ret) * int64(C(cnt[p]-1, cnt[ls[p]])) * int64(retl) % M)
			cnt[p] -= cnt[ls[p]]
		}
		if rs[p] > 0 {
			retr := dfs(rs[p])
			ret = int(int64(ret) * int64(C(cnt[p]-1, cnt[rs[p]])) * int64(retr) % M)
			cnt[p] -= cnt[rs[p]]
		}
		if ls[p] > 0 {
			cnt[p] += cnt[ls[p]]
		}
		if rs[p] > 0 {
			cnt[p] += cnt[rs[p]]
		}
		return ret
	}

	ans = dfs(1)
	return ans - 1
}

func main() {
	fmt.Println(numOfWays([]int{2, 1, 3}))
}
