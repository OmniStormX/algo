// https://leetcode.cn/problems/count-islands-with-total-value-divisible-by-k
package main

import "fmt"

type Queue[T any] struct {
	x     []T
	start int
	end   int
	size  int
	cap   int
}

func newQueue[T any]() *Queue[T] {
	return &Queue[T]{
		x:     make([]T, 1),
		start: 0,
		end:   0,
		size:  0,
		cap:   1,
	}
}

func (q *Queue[T]) Expand() {
	tmp := make([]T, 2*q.cap)
	for i := 0; i < q.size; i++ {
		tmp[i] = q.x[(q.start+i)%q.cap]
	}
	q.cap <<= 1
	q.x = tmp
	q.start = 0
	q.end = q.size
}

func (q *Queue[T]) Push(x T) {
	if q.size == q.cap {
		q.Expand()
	}

	q.x[q.end] = x
	q.end = (q.end + 1) % q.cap
	q.size++
}

func (q *Queue[T]) Pop() (x T, err error) {
	if q.size == 0 {
		err = fmt.Errorf("Queue size = 0, cannot pop item.")
		return
	}
	x = q.x[q.start]
	err = nil
	q.size--
	q.start = (q.start + 1) % q.cap
	return
}

func countIslands(grid [][]int, k int) int {
	n, m := len(grid), len(grid[0])
	vis := make([][]bool, n)
	for i := 0; i < n; i++ {
		vis[i] = make([]bool, m)
	}
	f := func(x, y int) int {
		q := newQueue[int]()
		dx := []int{0, 0, 1, -1}
		dy := []int{1, -1, 0, 0}
		q.Push(x*m + y)
		sum := grid[x][y]
		vis[x][y] = true
		for q.size > 0 {
			p, _ := q.Pop()
			x1, y1 := p/m, p%m
			for i := 0; i < 4; i++ {
				x2, y2 := x1+dx[i], y1+dy[i]
				if x2 < 0 || x2 >= n || y2 < 0 || y2 >= m || grid[x2][y2] == 0 || vis[x2][y2] == true {
					continue
				}
				vis[x2][y2] = true
				q.Push(x2*m + y2)
				sum += grid[x2][y2]
			}
		}
		return sum
	}
	ans := 0
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if grid[i][j] == 0 || vis[i][j] == true {
				continue
			}
			ff := f(i, j)
			if ff%k == 0 {
				ans++
			}
		}
	}
	return ans
}
