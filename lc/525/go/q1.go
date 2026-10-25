// https://leetcode.cn/problems/binary-digit-score
package main

func calculateScore(a int, b int) int {
	c1, c2 := 0, 0
	for i := 0; i < 31; i++ {
		c1 += a >> i & 1
		c2 += b >> i & 1
	}
	if c1 >= c2 {
		return c1 * c2
	}
	return c2 / c1
}
