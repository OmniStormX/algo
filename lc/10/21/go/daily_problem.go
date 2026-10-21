// https://leetcode.cn/problems/minimum-operations-to-sort-a-string
package main

func minOperations(s string) int {
	n := len(s)
	if n == 1 {
		return 0
	}
	mx, mn := -1, 26
	cnt := [26]int{}
	f := true
	for i := 0; i < n; i++ {
		c := int(s[i] - 'a')
		if mx < c {
			mx = c
		}
		if mn > c {
			mn = c
		}
		if i > 0 {
			if c < int(s[i-1]-'a') {
				f = false
			}
		}
		cnt[c]++
	}
	if f == true {
		return 0
	}
	start, end := int(s[0]-'a'), int(s[n-1]-'a')
	if start == mx && end == mn {
		if n == 2 {
			return -1
		}
		if cnt[start] > 1 || cnt[end] > 1 {
			return 2
		}
		return 3
	}

	if (start == mx && cnt[start] == 1) || (end == mn && cnt[end] == 1) {
		return 2
	}
	if start == mn || end == mx {
		return 1
	}
	return 2
}
