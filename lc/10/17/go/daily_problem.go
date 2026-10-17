// https://leetcode.cn/problems/minimum-swaps-to-sort-by-digit-sum
package main

import "sort"

type pair struct {
	v    int64
	from int
}

func minSwaps(nums []int) int {
	n := len(nums)
	d := make([]pair, n)
	for i, num := range nums {
		x := int64(num)
		for x > 0 {
			d[i].v += x % 10
			x /= 10
		}
		d[i].v *= 10000000000
		d[i].v += int64(num)
		d[i].from = i
	}
	sort.Slice(d, func(i, j int) bool {
		if d[i].v != d[j].v {
			return d[i].v < d[j].v
		}
		return d[i].from < d[j].from
	})

	ans := 0
	vis := make([]bool, n)
	for i := 0; i < n; i++ {
		if vis[i] {
			continue
		}
		l := 0
		p := i
		for vis[p] == false {
			vis[p] = true
			l++
			p = d[p].from
		}
		ans += l - 1
	}
	return ans
}
