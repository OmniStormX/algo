// https://leetcode.cn/problems/count-days-spent-together
package main

func countDaysTogether(arriveAlice string, leaveAlice string, arriveBob string, leaveBob string) int {
	month_days := []int{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}

	parse := func(str string) (int, int) {
		x := 0
		y := 0
		x = int(str[0]-'0')*10 + int(str[1]-'0')
		y = int(str[3]-'0')*10 + int(str[4]-'0')
		return x - 1, y
	}

	get := func(x, y int) int {
		ans := 0
		for i := 0; i < x; i++ {
			ans += month_days[i]
		}
		ans += y
		return ans
	}

	x1, y1 := parse(arriveAlice)
	x2, y2 := parse(leaveAlice)
	x3, y3 := parse(arriveBob)
	x4, y4 := parse(leaveBob)

	sum1 := get(x1, y1)
	sum2 := get(x2, y2)
	sum3 := get(x3, y3)
	sum4 := get(x4, y4)
	ans := sum2 - sum1 + 1 + sum4 - sum3 + 1 - (max(sum2, sum4) - min(sum1, sum3) + 1)
	ans = max(ans, 0)
	return ans
}
