// https://leetcode.cn/problems/gas-station
package main

func canCompleteCircuit(gas []int, cost []int) int {
	n := len(gas)

	sum := 0
	mx := 0
	p := 0
	for i := 0; i < n; i++ {
		sum = sum + gas[i] - cost[i]
		if sum < mx {
			mx = sum
			p = i + 1
		}
	}
	if sum < 0 {
		return -1
	}
	return p
}
