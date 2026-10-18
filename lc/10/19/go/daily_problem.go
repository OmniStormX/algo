// https://leetcode.cn/problems/find-target-indices-after-sorting-array
package main

func targetIndices(nums []int, target int) []int {
	ans := make([]int, 0)

	l1, r1 := 0, len(nums)-1

	for i := 0; i < len(nums); i++ {
		if nums[i] < target {
			l1++
		} else if nums[i] > target {
			r1--
		}
	}
	for i := l1; i <= r1; i++ {
		ans = append(ans, i)
	}
	return ans
}
