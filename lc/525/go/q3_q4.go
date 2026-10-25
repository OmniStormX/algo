// https://leetcode.cn/contest/weekly-contest-525/problems/count-frequency-staircase-subarrays-ii
package main

var cnt []int = make([]int, 100009)
var dd []int = make([]int, 100009)

func countStaircase(nums []int) int {
	n := len(nums)
	d := make([]int, 0)
	for i := 1; i <= n; i++ {
		if i*(i+1)/2 > n {
			break
		}
		d = append(d, i*(i+1)/2)
	}
	ans := 0
	for i := 0; i < n; i++ {
		for id := 1; i+id*(id+1)/2 <= n; id++ {
			l := id * (id + 1) / 2
			if i+l-1 >= n {
				break
			}
			m := 1
			f := true
			j := i
			s := 0
			for j = i; j < i+l; j++ {
				cnt[nums[j]]++
				if dd[nums[j]] == 0 {
					dd[nums[j]] = m
					m++
					if m > id+1 {
						f = false
						j++
						break
					}
				}
				if cnt[nums[j]] > dd[nums[j]] {
					j++
					f = false
					break
				} else if cnt[nums[j]] == dd[nums[j]] {
					s++
				}

			}
			if m != id+1 || s != id {
				f = false
			}
			for q := i; q < j; q++ {
				dd[nums[q]] = 0
				cnt[nums[q]] = 0
			}
			if f {
				ans++
			}
		}
	}
	return ans
}
