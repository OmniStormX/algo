package main

func sumCounts(nums []int) int {
	ans := 0
	n := len(nums)
	cnt := [101]int{0}
	sum := 0
	for i := 0; i < n; i++ {
		for j := i; j < n; j++ {
			if cnt[nums[j]] == 0 {
				sum++
			}
			cnt[nums[j]]++
			ans += sum * sum
		}
		for j := 0; j < n; j++ {
			cnt[nums[j]] = 0
		}
		sum = 0
	}
	return ans
}
