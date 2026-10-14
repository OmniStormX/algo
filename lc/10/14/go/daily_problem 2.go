package main

import "strconv"

func maxSumOfSquares2(num int, sum int) string {
	l := num
	if l*9 < sum {
		return ""
	}
	ans := make([]byte, 0)
	q := 0
	for q < l {
		if sum <= 9 {
			ans = strconv.AppendInt(ans, int64(sum), 10)
			sum = 0
		} else {
			ans = strconv.AppendInt(ans, int64(9), 10)
			sum -= 9
		}
		q++
	}
	return string(ans)
}
