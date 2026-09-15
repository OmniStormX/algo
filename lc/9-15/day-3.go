package main

func getSum(a int, b int) int {

	for b != 0 {
		c := (a & b) << 1
		a = a ^ b
		b = c
	}

	return a
}
