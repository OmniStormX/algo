// https://leetcode.cn/problems/minimum-remove-to-make-valid-parentheses/
package main

func minRemoveToMakeValid(s string) string {
	n := len(s)
	cur := 0
	t := make([]bool, n)
	for i := 0; i < n; i++ {
		t[i] = true
	}
	for i := 0; i < n; i++ {
		if s[i] == '(' {
			cur++
		} else if s[i] == ')' {
			if cur == 0 {
				t[i] = false
				continue
			}
			cur--
		}
	}
	cur = 0
	for i := n - 1; i >= 0; i-- {
		if t[i] == false {
			continue
		}
		if s[i] == '(' {
			if cur == 0 {
				t[i] = false
				continue
			}
			cur++
		} else if s[i] == ')' {
			cur--
		}
	}

	ans := make([]byte, 0)

	for i := 0; i < n; i++ {
		if t[i] == true {
			ans = append(ans, s[i])
		}
	}

	return string(ans)
}
