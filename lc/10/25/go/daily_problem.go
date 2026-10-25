// https://leetcode.cn/problems/number-of-closed-islands
package main

func closedIsland(grid [][]int) int {
	n, m := len(grid), len(grid[0])

	vis := make([][]bool, n)
	for i := 0; i < n; i++ {
		vis[i] = make([]bool, m)
	}

	type pair struct {
		x int
		y int
	}

	dx := []int{0, 0, 1, -1}
	dy := []int{1, -1, 0, 0}

	bfs := func(i, j int) {
		if vis[i][j] == true || grid[i][j] == 1 {
			return
		}
		q := make([]pair, 0)
		q = append(q, pair{i, j})
		vis[i][j] = true
		for len(q) > 0 {
			l := len(q)
			tmp := make([]pair, 0)
			for i := 0; i < l; i++ {
				d := q[len(q)-1]
				q = q[:len(q)-1]
				// vis[d.x][d.y] = true
				for k := 0; k < 4; k++ {
					x1, y1 := d.x+dx[k], d.y+dy[k]
					if x1 < 0 || x1 >= n || y1 < 0 || y1 >= m || grid[x1][y1] == 1 || vis[x1][y1] == true {
						continue
					}
					vis[x1][y1] = true
					tmp = append(tmp, pair{x1, y1})
				}
			}
			q = tmp
		}
	}
	ans := 0
	for i := 0; i < m; i++ {
		bfs(0, i)
		bfs(n-1, i)
	}
	for i := 0; i < n; i++ {
		bfs(i, 0)
		bfs(i, m-1)
	}
	// fmt.Println(vis)
	for i := 1; i < n-1; i++ {
		for j := 0; j < m-1; j++ {
			if vis[i][j] == false && grid[i][j] == 0 {
				bfs(i, j)
				ans++
			}
		}
	}
	return ans
}
