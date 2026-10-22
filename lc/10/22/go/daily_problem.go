// https://leetcode.cn/problems/minimum-operations-to-sort-a-permutation/description
package main

func minOperations(nums []int) int {
	n := len(nums)
	if n == 1 {
		return 0
	}

	for i := 0; i < n; i++ {
		ab := nums[i] - nums[(i-1+n)%n]
		if ab < 0 {
			ab *= -1
		}
		if !(ab == 1 || ab == n-1) {
			return -1
		}
	}

	f := true
	for i := 1; i < n; i++ {
		if nums[i]-nums[i-1] != 1 {
			f = false
			break
		}
	}
	if f == true {
		return 0
	}

	f = true
	for i := 1; i < n; i++ {
		if nums[i]-nums[i-1] != -1 {
			f = false
			break
		}
	}
	if f == true {
		return 1
	}

	mx := -1
	for i := 0; i < n; i++ {
		if nums[i] == n-1 {
			mx = i
		}
	}

	if nums[mx]-nums[(mx-1+n)%n] == 1 {
		mn := 2 + n - (mx + 1)
		return min(mn, mx+1)
	} else if nums[mx]-nums[(mx+1+n)%n] == 1 {
		mn := mx + 1
		mx := n - 1 - mx
		return min(mn, mx+2)
	}
	return -1
}
