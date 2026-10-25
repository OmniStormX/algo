// https://leetcode.cn/problems/maximize-sum-after-at-most-k-digit-rearrangements/
package main

import "sort"

type numArray struct {
	x    []int
	id   int
	diff int
}

func maximizeSum(nums []int, k int) int64 {
	n := len(nums)
	a := make([]numArray, n)

	for i := 0; i < n; i++ {
		a[i].id = i
		x := nums[i]
		for x > 0 {
			a[i].x = append(a[i].x, x%10)
			x /= 10
		}
		sort.Slice(a[i].x, func(x1, x2 int) bool {
			return a[i].x[x1] > a[i].x[x2]
		})
		for _, x := range a[i].x {
			a[i].diff = a[i].diff*10 + x
		}
		a[i].diff -= nums[i]
	}

	sort.Slice(a, func(i, j int) bool {
		return a[i].diff > a[j].diff
	})
	ans := int64(0)
	for _, x := range nums {
		ans += int64(x)
	}
	for j := range k {
		ans += int64(a[j].diff)
	}
	return ans
}
