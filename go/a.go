package main

import (
	"container/list"
)

type State struct {
	p int
	e int
	s int
}

type Pair struct {
	first  int
	second int
}

func minMoves(classroom []string, energy int) int {
	n, m := len(classroom), len(classroom[0])
	mp := make(map[Pair]int)

	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if classroom[i][j] == 'L' {
				L := len(mp)
				mp[Pair{first: i, second: j}] = L
			}
		}
	}

	ll := len(mp)
	if len(mp) == 0 {
		return 0
	}
	var state = func(x, e, hasCollect int) int {
		return (e << (ll + 10)) | (x << 10) | hasCollect
	}

	curx, cury := 0, 0
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			if classroom[i][j] == 'S' {
				curx, cury = i, j
			}
		}
	}

	queue := list.New()
	queue.PushBack(State{
		p: curx*m + cury,
		e: energy,
		s: 0,
	})

	Inf := 1000_000_000
	dis := make(map[int]int)

	dis[state(curx*m+cury, energy, 0)] = 0

	dx := []int{0, 0, 1, -1}
	dy := []int{1, -1, 0, 0}

	for queue.Len() > 0 {
		front, _ := queue.Front().Value.(State)

		p, e, s := front.p, front.e, front.s
		// if s == (1<<ll)-1 {
		// 	break
		// }
		x := p / m
		y := p % m
		queue.Remove(queue.Front())
		// fmt.Printf("queue:\tx = %d, y = %d, e = %d, s = %d\n", x, y, e, s)
		for j := 0; j < 4; j++ {
			xx, yy := x+dx[j], y+dy[j]
			if xx >= 0 && e > 0 && xx < n && yy >= 0 && yy < m && classroom[xx][yy] != 'X' {
				ss := s
				if classroom[xx][yy] == 'L' {
					ss |= (1 << (mp[Pair{first: xx, second: yy}]))
				}
				ee := e - 1
				if classroom[xx][yy] == 'R' {
					ee = energy
				}
				// fmt.Printf("xx = %d, yy = %d, ee = %d, ss = %d\n", xx, yy, ee, ss)
				newK := state(xx*m+yy, ee, ss)
				oldK := state(x*m+y, e, s)
				if _, ok := dis[newK]; ok {
					if dis[newK] > dis[oldK]+1 {
						dis[newK] = dis[oldK] + 1
						queue.PushBack(State{
							p: xx*m + yy,
							e: ee,
							s: ss,
						})
					}
				} else {
					dis[newK] = dis[oldK] + 1
					queue.PushBack(State{
						p: xx*m + yy,
						e: ee,
						s: ss,
					})

				}
			}

		}

	}

	Min := Inf
	for i := 0; i < n; i++ {
		for j := 0; j < m; j++ {
			for e := 0; e <= energy; e++ {
				if classroom[i][j] == 'L' {
					if _, ok := dis[state(i*m+j, e, (1<<len(mp))-1)]; ok {
						Min = min(Min, dis[state(i*m+j, e, (1<<len(mp))-1)])
					}
				}
			}

		}
	}

	if Min == Inf {
		return -1
	}
	return Min
}

// func main() {
// 	cl := []string{"S.", "XL"}
// 	energy := 2
// 	// fmt.Println(minMoves(cl, energy))
// 	// cl = []string{"LS", "RL"}
// 	// energy = 4
// 	// fmt.Println(minMoves(cl, energy))
// 	cl = []string{"RL", "S."}
// 	energy = 1
// 	fmt.Println(minMoves(cl, energy))
// 	// cl = []string{"SL."}
// 	// energy = 2
// 	// fmt.Println(minMoves(cl, energy))

// }
