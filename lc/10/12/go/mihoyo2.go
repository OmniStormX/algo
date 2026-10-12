// https://leetcode.cn/problems/minimum-remove-to-make-valid-parentheses/
package main

func minRemoveToMakeValid2(s string) string {
	// use stack
	n := len(s)
	stk := make([]int, 0, n)
	cur := 0
	t := make([]bool, n)
	for i := 0; i < n; i++ {
		t[i] = true
	}
	for i := 0; i < n; i++ {
		if s[i] == '(' {
			cur++
			stk = append(stk, i)
		} else if s[i] == ')' {
			if cur == 0 {
				t[i] = false
				continue
			}
			cur--
			stk = stk[:len(stk)-1]
		}
	}
	for _, x := range stk {
		t[x] = false
	}

	ans := make([]byte, 0)
	for i := 0; i < n; i++ {
		if t[i] == true {
			ans = append(ans, s[i])
		}
	}
	return string(ans)

}
