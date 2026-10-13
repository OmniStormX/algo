package main

import "sort"

func maxAlternatingSum(nums []int) int64 {
	n := len(nums)
	for i := 0; i < n; i++ {
		if nums[i] < 0 {
			nums[i] = -nums[i]
		}
	}
	sort.Ints(nums)
	ans := int64(0)
	for i := 0; i < n; i++ {
		if (i+1)*2 <= n {
			ans -= int64(nums[i]) * int64(nums[i])
		} else {
			ans += int64(nums[i]) * int64(nums[i])
		}
	}
	return ans
}
