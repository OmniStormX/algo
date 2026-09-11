package main

import (
	"bufio"
	"fmt"
	"os"
)

const INF int64 = 1 << 60

// ============================================================
// Info：节点维护的信息
// ============================================================

type Info struct {
	sum int64
	len int
}

// Info + Info
func merge(a, b Info) Info {
	if a.len == 0 {
		return b
	}
	if b.len == 0 {
		return a
	}

	return Info{
		sum: (a.sum + b.sum) % M,
		len: a.len + b.len,
	}
}

func e() Info {
	return Info{
		sum: 0,
		len: 0,
	}
}

// ============================================================
// Tag：懒标记
// ============================================================

type Tag struct {
	add int64
	mul int64
}

// 空 Tag
func id() Tag {
	return Tag{add: 0, mul: 1}
}

// Info + Tag
//
// 将 f 作用到整个区间的信息 x 上
func apply(f Tag, x Info) Info {
	if x.len == 0 {
		return x
	}

	if f.mul != 1 {
		x.sum = x.sum * f.mul % M
	}
	x.sum = (x.sum + f.add*int64(x.len)) % M

	return x
}

// Tag + Tag
//
// composition(f, g)
// 表示：先执行 g，再执行 f
//
// 即：
// x --g--> g(x) --f--> f(g(x))
func composition(f, g Tag) Tag {
	return Tag{
		add: (f.add + g.add*f.mul) % M,
		mul: f.mul * g.mul % M,
	}
}

var M int64

// ============================================================
// ZKW Lazy Segment Tree
// ============================================================

type LazySegTree struct {
	n    int
	size int
	log  int

	d  []Info
	lz []Tag
}

func NewLazySegTree(a []int64) *LazySegTree {
	n := len(a)

	size := 1
	log := 0
	for size < n {
		size <<= 1
		log++
	}

	d := make([]Info, size<<1)
	lz := make([]Tag, size)

	for i := range d {
		d[i] = e()
	}

	for i := range lz {
		lz[i] = id()
	}

	// leaves
	for i := 0; i < n; i++ {
		d[size+i] = Info{
			sum: a[i],
			len: 1,
		}
	}

	// empty leaves still need len = 0
	for i := size - 1; i >= 1; i-- {
		d[i] = merge(d[i<<1], d[i<<1|1])
	}

	return &LazySegTree{
		n:    n,
		size: size,
		log:  log,
		d:    d,
		lz:   lz,
	}
}

// d[k] = d[2k] + d[2k+1]
func (st *LazySegTree) update(k int) {
	st.d[k] = merge(st.d[k<<1], st.d[k<<1|1])
}

// 给整个节点 k 打 Tag
func (st *LazySegTree) allApply(k int, f Tag) {
	st.d[k] = apply(f, st.d[k])

	if k < st.size {
		st.lz[k] = composition(f, st.lz[k])
	}
}

// 下推
func (st *LazySegTree) push(k int) {
	st.allApply(k<<1, st.lz[k])
	st.allApply(k<<1|1, st.lz[k])

	st.lz[k] = id()
}

// ============================================================
// 单点
// ============================================================

func (st *LazySegTree) Set(p int, x int64) {
	p += st.size

	for i := st.log; i >= 1; i-- {
		st.push(p >> i)
	}

	st.d[p] = Info{
		sum: x,
		len: 1,
	}

	for i := 1; i <= st.log; i++ {
		st.update(p >> i)
	}
}

func (st *LazySegTree) Get(p int) Info {
	p += st.size

	for i := st.log; i >= 1; i-- {
		st.push(p >> i)
	}

	return st.d[p]
}

// ============================================================
// 区间查询 [l, r)
// ============================================================

func (st *LazySegTree) Prod(l, r int) Info {
	if l >= r {
		return e()
	}

	l += st.size
	r += st.size

	// 将边界上的 lazy 下推
	for i := st.log; i >= 1; i-- {
		if ((l >> i) << i) != l {
			st.push(l >> i)
		}

		if ((r >> i) << i) != r {
			st.push((r - 1) >> i)
		}
	}

	sml := e()
	smr := e()

	for l < r {
		if l&1 != 0 {
			sml = merge(sml, st.d[l])
			l++
		}

		if r&1 != 0 {
			r--
			smr = merge(st.d[r], smr)
		}

		l >>= 1
		r >>= 1
	}

	return merge(sml, smr)
}

// 整棵树
func (st *LazySegTree) AllProd() Info {
	return st.d[1]
}

// ============================================================
// 区间修改 [l, r) += x
// ============================================================

func (st *LazySegTree) RangeAdd(l, r int, x int64) {
	st.RangeApply(l, r, Tag{add: x})
}

func (st *LazySegTree) RangeApply(l, r int, f Tag) {
	if l >= r {
		return
	}

	l += st.size
	r += st.size

	l0 := l
	r0 := r

	// push boundary
	for i := st.log; i >= 1; i-- {
		if ((l >> i) << i) != l {
			st.push(l >> i)
		}

		if ((r >> i) << i) != r {
			st.push((r - 1) >> i)
		}
	}

	for l < r {
		if l&1 != 0 {
			st.allApply(l, f)
			l++
		}

		if r&1 != 0 {
			r--
			st.allApply(r, f)
		}

		l >>= 1
		r >>= 1
	}

	l = l0
	r = r0

	// pull
	for i := 1; i <= st.log; i++ {
		if ((l >> i) << i) != l {
			st.update(l >> i)
		}

		if ((r >> i) << i) != r {
			st.update((r - 1) >> i)
		}
	}
}

// ============================================================
// 线段树二分：MaxRight
// ============================================================
//
// 从 l 开始寻找最大的 r，使：
//
//      pred(Prod(l, r)) == true
//
// 返回 r
//
// 要求：
//      pred(e()) == true
//
// 并且 pred 对区间扩展具有单调性。
//
// 典型：
//      找第一个使 sum >= K 的位置
//
// pred := func(x Info) bool {
//     return x.sum < K
// }
//
// p := st.MaxRight(l, pred)
//
// p 就是第一个使前缀 sum >= K 的位置。
// ============================================================

func (st *LazySegTree) MaxRight(l int, pred func(Info) bool) int {
	if l == st.n {
		return st.n
	}

	l += st.size

	for i := st.log; i >= 1; i-- {
		st.push(l >> i)
	}

	sm := e()

	for {
		for l&1 == 0 {
			l >>= 1
		}

		nxt := merge(sm, st.d[l])

		if !pred(nxt) {
			// 下沉
			for l < st.size {
				st.push(l)

				l <<= 1
				nxt = merge(sm, st.d[l])

				if pred(nxt) {
					sm = nxt
					l++
				}
			}

			return l - st.size
		}

		sm = nxt
		l++

		if l&-l == l {
			break
		}
	}

	return st.n
}

// ============================================================
// 线段树二分：MinLeft
// ============================================================
//
// 从 r 向左寻找最小的 l，使：
//
//      pred(Prod(l, r)) == true
//
// 返回 l
//
// 要求：
//      pred(e()) == true
//
// ============================================================

func (st *LazySegTree) MinLeft(r int, pred func(Info) bool) int {
	if r == 0 {
		return 0
	}

	r += st.size

	for i := st.log; i >= 1; i-- {
		st.push((r - 1) >> i)
	}

	sm := e()

	for {
		r--

		for r > 1 && r&1 != 0 {
			r >>= 1
		}

		nxt := merge(st.d[r], sm)

		if !pred(nxt) {
			for r < st.size {
				st.push(r)

				r = r<<1 | 1
				nxt = merge(st.d[r], sm)

				if pred(nxt) {
					sm = nxt
					r--
				}
			}

			return r + 1 - st.size
		}

		sm = nxt

		if r&-r == r {
			break
		}
	}

	return 0
}

func main() {
	in := bufio.NewReaderSize(os.Stdin, 1<<20)
	n, q, m := 0, 0, 0
	fmt.Fscan(in, &n, &q, &m)
	M = int64(m)
	a := make([]int64, n+1)
	for i := 0; i < n; i++ {
		fmt.Fscan(in, &a[i])
	}
	seg := NewLazySegTree(a)

	for i := 0; i < q; i++ {
		// fmt.Println("i = ", i)
		op := 0
		fmt.Fscan(in, &op)
		if op == 1 {
			var x, y, k int
			fmt.Fscan(in, &x, &y, &k)
			x--
			y--
			seg.RangeApply(x, y+1, Tag{
				mul: int64(k),
				add: 0,
			})
		} else if op == 2 {
			var x, y, k int
			fmt.Fscan(in, &x, &y, &k)
			x--
			y--
			seg.RangeApply(x, y+1, Tag{
				add: int64(k),
				mul: 1,
			})
		} else {
			var x, y int
			fmt.Fscan(in, &x, &y)
			x--
			y--
			fmt.Println(seg.Prod(x, y+1).sum)
		}
	}
}

/*
5 5 38
1 5 4 2 3
2 1 4 1
3 2 5
1 2 4 2
2 3 5 5
3 1 4

*/
