// https://leetcode.cn/problems/integer-to-roman

package main

import (
	"fmt"
	"strconv"
)

func intToRoman(num int) string {
	ans := ""
	nums := strconv.Itoa(num)
	n := len(nums)
	R := [][]string{
		{"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
		{"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
		{"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
		{"", "M", "MM", "MMM"},
	}
	fmt.Println(nums)
	for i := 0; i < n; i++ {
		// fmt.Println(int(nums[n - 1 - i] - '0'))
		ans += R[n-1-i][int(nums[i]-'0')]
	}
	return ans
}
