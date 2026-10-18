// https://leetcode.cn/problems/number-of-ways-to-reorder-array-to-get-same-bst
// solve in O(n) time limit
package main

const M = 1000000007

func numOfWays_2(nums []int) int {
	// C_n^i
	n := len(nums)
	prev := make([]int, n+2)
	next := make([]int, n+2)
	size := make([]int, n+2)

	for i := 0; i <= n; i++ {
		prev[i] = i - 1
		next[i] = i + 1
	}
	// s0 : size of father
	// s1 : size of left son
	// s2 : size of right son
	// d_f = C_{s0 - 1}^{s1} * d_{s1} * d_{s2}
	// d_f = (s0 - 1)! / (s1! * s2!) * d_{s1} * d_{s2}
	// d_f / (s0 - 1)! = d_{s1} / s1! * d_{s2} / s2!
	// s0 * d_f / (s0)! = d_{s1} / s1! * d_{s2} / s2!

	// 根 > 叶
	fac, prod := 1, 1
	for i := n - 1; i >= 0; i-- {
		fac = fac * (i + 1) % M
		x := nums[i]
		size[x] = 1
		left := prev[x]
		if size[left] > 0 {
			size[x] += size[left]
			pl := prev[left]
			prev[x] = pl
			next[pl] = x
		}

		right := next[x]
		if size[right] > 0 {
			size[x] += size[right]
			nr := next[right]
			prev[nr], next[x] = x, nr
		}
		prod = prod * size[x] % M
	}

	return (fac*pow(prod, M-2) - 1 + M) % M
}

func pow(x, n int) int {
	res := 1
	for ; n > 0; n /= 2 {
		if n%2 > 0 {
			res = res * x % M
		}
		x = x * x % M
	}
	return res
}
