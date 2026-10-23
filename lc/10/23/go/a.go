// pdd 面试, https://leetcode.cn/problems/implement-rand10-using-rand7/
package main

import (
	"math/rand"
)

func rand7() int {
	return rand.Int()%7 + 1
}

func rand10() int {

	x := 48
	for x >= 40 {
		x = (rand7()-1)*7 + rand7() - 1
	}
	return x%10 + 1
}

// func main() {
// 	cnt := make([]int, 11)

// 	for range 10000 {
// 		cnt[rand10()]++
// 	}
// 	fmt.Println(cnt)
// }
