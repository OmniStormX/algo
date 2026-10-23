// https://leetcode.cn/problems/maximum-k-to-sort-a-permutation
package main

func sortPermutation(nums []int) int {
	ans := (1 << 31) - 1

	n := len(nums)
	vis := make([]bool, n)
	for i := 0; i < n; i++ {
		if vis[i] == true {
			continue
		}
		if nums[i] == i {
			continue
		}

		x := nums[i]
		d := nums[i]
		for x != nums[i] {
			d &= x
			x = nums[x]
		}
		ans &= d
	}
	if ans == (1<<31)-1 {
		return 0
	}
	return ans
}
