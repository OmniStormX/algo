package main

import "fmt"

func main() {
	var ps []*int
	for i := 0; i < 3; i++ {
		ps = append(ps, &i)
	}
	for _, p := range ps {
		fmt.Println(*p, p)
	}
}
