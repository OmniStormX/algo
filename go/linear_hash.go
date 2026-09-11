package main

type XorShift64 struct {
	x uint64
}

func NewXorShift64(seed uint64) *XorShift64 {
	if seed == 0 {
		seed = 1
	}
	return &XorShift64{x: seed}
}

func (r *XorShift64) Next() uint64 {
	x := r.x
	x ^= x << 13
	x ^= x >> 17
	x ^= x << 5
	r.x = x
	return x
}

type Hash struct {
	h    []uint64
	pow  []uint64
	base uint64
}

func newHash(nums []int, base uint64) *Hash {
	n := len(nums)
	d := make(map[int]uint64)
	sd := NewXorShift64(0x3f)
	for i := 0; i < n; i++ {
		if _, ok := d[nums[i]]; !ok {
			d[nums[i]] = sd.Next()
		}
	}
	h := make([]uint64, n+1)
	pow := make([]uint64, n+1)
	pow[0] = 1
	for i := 1; i <= n; i++ {
		h[i] = h[i-1]*base + d[nums[i-1]]
		pow[i] = pow[i-1] * base
	}
	return &Hash{
		h:   h,
		pow: pow,
	}
}

// get hash from [l, r]
func (h *Hash) getHash(l, r int) uint64 {
	return h.h[r] - h.h[l-1]*h.pow[r-l+1]
}
