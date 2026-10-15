package main

func sumCounts(nums []int) int {
	ans := 0
	n := len(nums)
	cnt := [101]int{0}
	for i := 0; i < n; i++ {
		for j := i; j < n; j++ {
			sum := 0
			for k := i; k <= j; k++ {
				if cnt[nums[k]] == 0 {
					sum++
				}
				cnt[nums[k]]++
			}
			ans += sum * sum
			for k := i; k <= j; k++ {
				cnt[nums[k]]--
			}
		}
	}
	return ans
}
