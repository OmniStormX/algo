package main

func maxSum(nums []int, k int) int {
	n := len(nums)

	d := make([]int, 40)
	base := int64(1) << 31
	for i := 0; i < n; i++ {
		x := base + int64(nums[i])
		j := 0
		for x > 0 {
			d[j] += int(x & 1)
			x >>= 1
			j++
		}
	}

	M := 1e9 + 7
	ans := int64(0)

	for i := 0; i < k; i++ {
		x := int64(0)
		for j := 0; j < 31; j++ {
			if d[j] > 0 {
				d[j]--
				x |= (1 << j)
			}
		}
		ans = (ans + int64(x)*x) % int64(M)
	}
	return int(ans)
}
