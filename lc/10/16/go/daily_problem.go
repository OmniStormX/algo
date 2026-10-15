// https://leetcode.cn/problems/subarrays-distinct-element-sum-of-squares-ii
package main

import "fmt"

const M = 1e9 + 7

type node struct {
	l    int
	r    int
	val  int64
	val2 int64
	tag  int
}

type Seg struct {
	t []node
	n int
}

// func (s *Seg) build(p int, l int, r int) {

// }

func (s *Seg) build(p, l, r int) {
	s.t[p].l = l
	s.t[p].r = r

	if l == r {
		return
	}

	m := (l + r) / 2
	s.build(p*2, l, m)
	s.build(p*2+1, m+1, r)
}

func newSeg(n int) *Seg {
	s := &Seg{
		t: make([]node, n*4+9),
		n: n,
	}
	s.build(1, 0, n-1)
	return s
}

func (s *Seg) RangeApply(p, l, r, ql, qr, y int) {
	if ql <= l && r <= qr {
		s.t[p].addTag(y)
		fmt.Println("rangeModify: l = ", l, ", r = ", r, ", val2 = ", s.t[p].val2)
		return
	}
	s.pushdown(p)
	m := (l + r) / 2
	if m >= ql {
		s.RangeApply(p*2, l, m, ql, qr, y)
	}
	if m < qr {
		s.RangeApply(p*2+1, m+1, r, ql, qr, y)
	}
	s.t[p] = *s.t[p].merge(&s.t[p*2], &s.t[p*2+1])
}

func (s *Seg) RangeQuery(p, l, r, ql, qr int) int64 {
	if ql <= l && r <= qr {
		fmt.Println("rangeQuery: l = ", l, ", r = ", r, ", val2 = ", s.t[p].val2)
		return s.t[p].val2
	}
	m := (l + r) / 2
	s.pushdown(p)
	ans := int64(0)
	if m >= ql {
		ans += s.RangeQuery(p*2, l, m, ql, qr)
	}
	if m < qr {
		ans += s.RangeQuery(p*2+1, m+1, r, ql, qr)
	}
	return ans % M
}

func (n *node) addTag(t int) {
	n.tag += t
	n.val2 += n.val*2*int64(t) + int64(n.r-n.l+1)*int64(t)*int64(t)
	n.val2 %= M
	n.val += int64(n.r-n.l+1) * int64(t)
	n.val %= M
}

func (s *Seg) pushdown(p int) {
	if s.t[p].tag != 0 {
		s.t[p*2].addTag(s.t[p].tag)
		s.t[p*2+1].addTag(s.t[p].tag)
		s.t[p].tag = 0
	}
}

func (n *node) merge(a, b *node) *node {
	return &node{
		l:    a.l,
		r:    b.r,
		val:  (a.val + b.val) % M,
		val2: (a.val2 + b.val2) % M,
		tag:  0,
	}
}

func sumCounts(nums []int) int {

	last := make(map[int]int)

	n := len(nums)
	s := newSeg(n)
	ans := int64(0)
	for i := 0; i < n; i++ {
		if p, ok := last[nums[i]]; !ok {
			s.RangeApply(1, 0, n-1, 0, i, 1)
		} else {
			s.RangeApply(1, 0, n-1, p+1, i, 1)
		}
		last[nums[i]] = i

		ans += s.RangeQuery(1, 0, n-1, 0, i)
		ans %= M
	}
	return int(ans)
}
