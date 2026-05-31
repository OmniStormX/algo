package main

import (
	"container/heap"
)

type Item struct {
	value    int
	priority int
}

type PriorityQueue struct {
	items []Item
	size  int
	mem   map[int]int
}

func (pq *PriorityQueue) Len() int {
	return len(pq.items)
}

// x 小根堆，优先队列, 优先级低的先出
func (pq *PriorityQueue) Less(i, j int) bool {
	return pq.items[i].priority < pq.items[j].priority
}

func (pq *PriorityQueue) Swap(i, j int) {
	pq.items[i], pq.items[j] = pq.items[j], pq.items[i]
}

func (pq *PriorityQueue) Push(x interface{}) {
	item := x.(Item)
	pq.items = append(pq.items, item)
}

func (pq *PriorityQueue) push(item Item) {
	heap.Push(pq, item)
	if _, ok := pq.mem[item.value]; !ok {
		pq.mem[item.value] = 0
	}
	pq.size++
}

func (pq *PriorityQueue) Pop() interface{} {
	old := pq.items
	n := len(old)
	item := old[n-1]
	pq.items = old[:n-1]
	return item
}

func (pq *PriorityQueue) pop(val int) {
	pq.mem[val]++
	pq.size--
}

func (pq *PriorityQueue) Top() Item {
	pq.ApplyRemove()
	return pq.items[0]
}

func (pq *PriorityQueue) ApplyRemove() {
	for pq.Len() > 0 && pq.mem[pq.items[0].value] > 0 {
		val := pq.items[0].value
		heap.Pop(pq)
		pq.mem[val]--
	}
}

func isBipartite(graph [][]int) bool {
	var dfs func(int, int) bool
	n := len(graph)
	color := make([]int, n)
	for i := 0; i < n; i++ {
		color[i] = 0
	}

	dfs = func(u, t int) bool {
		color[u] = t
		for _, son := range graph[u] {
			if color[son] == t || (color[son] == 0 && !dfs(son, -t)) {
				return false
			}
		}
		return true
	}
	ans := true
	for i := 0; i < n; i++ {
		if color[i] != 0 {
			continue
		}
		if !dfs(i, i+1) {
			ans = false
			break
		}
	}
	return ans
}

// func main() {
// 	fmt.Println(isBipartite([][]int{{1, 2, 3}, {0, 2}, {0, 1, 3}, {0, 2}}))
// 	fmt.Println(isBipartite([][]int{{1, 3}, {0, 2}, {1, 3}, {0, 2}}))
// }
