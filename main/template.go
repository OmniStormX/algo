package main

import (
	"bufio"
	"container/heap"
	"fmt"
	"os"
	"reflect"
)

type i64 int64
type u64 uint64
type u32 uint32
type f64 float64
type f32 float32

type IO struct {
	Reader *bufio.Reader
	Writer *bufio.Writer
}

func NewIO() *IO {
	reader := bufio.NewReader(os.Stdin)
	writer := bufio.NewWriter(os.Stdout)
	return &IO{reader, writer}
}

func (io *IO) flush() {
	io.Writer.Flush()
}

func (io *IO) I() int64 {
	var x int64
	fmt.Fscan(io.Reader, &x)
	return x
}

func (io *IO) print(x interface{}) {
	fmt.Fprintln(io.Writer, x)
}

// Modular 模数运算接口
type Modular interface {
	Add(b Modular) Modular
	Sub(b Modular) Modular
	Mul(b Modular) Modular
	Inv() Modular
	Div(b Modular) Modular
	Val() int64
}

// ModInt 是 Modular 接口的实现
type ModInt struct {
	v int64
}

var MOD int64 = 998244353

// Z 创建一个新的 ModInt 实例
func Z(x int64) Modular {
	x %= MOD
	if x < 0 {
		x += MOD
	}
	return &ModInt{x}
}

// Add 实现 Modular 接口的 Add 方法
func (a *ModInt) Add(b Modular) Modular {
	v := a.v + b.Val()
	if v >= MOD {
		v -= MOD
	}
	return &ModInt{v}
}

// Sub 实现 Modular 接口的 Sub 方法
func (a *ModInt) Sub(b Modular) Modular {
	v := a.v - b.Val()
	if v < 0 {
		v += MOD
	}
	return &ModInt{v}
}

// Mul 实现 Modular 接口的 Mul 方法
func (a *ModInt) Mul(b Modular) Modular {
	return &ModInt{(a.v * b.Val()) % MOD}
}

// Pow 计算 Modular 的幂
func Pow(a Modular, b int64) Modular {
	res := Z(1)
	for b > 0 {
		if b&1 == 1 {
			res = res.Mul(a)
		}
	}
	return res
}

// Inv 实现 Modular 接口的 Inv 方法
func (a *ModInt) Inv() Modular {
	return Pow(a, MOD-2)
}

// Div 实现 Modular 接口的 Div 方法
func (a *ModInt) Div(b Modular) Modular {
	return a.Mul(b.Inv())
}

// Val 实现 Modular 接口的 Val 方法
func (a *ModInt) Val() int64 {
	return a.v
}

func Abs(x interface{}) interface{} {
	v := reflect.ValueOf(x)

	switch v.Kind() {
	case reflect.Int, reflect.Int8, reflect.Int16, reflect.Int32, reflect.Int64:
		val := v.Int()
		if val < 0 {
			// 返回对应类型的负数
			return reflect.ValueOf(-val).Convert(v.Type()).Interface()
		}
	case reflect.Float32, reflect.Float64:
		val := v.Float()
		if val < 0 {
			return reflect.ValueOf(-val).Convert(v.Type()).Interface()
		}
	}
	return x
}

type Fenwick struct {
	t []i64
}

func NewFenwick(n int) *Fenwick {
	return &Fenwick{t: make([]i64, n+1)}
}

func (f *Fenwick) Add(i int, x i64) {
	for i < len(f.t) {
		f.t[i] += x
		i += i & -i
	}
}

func (f *Fenwick) Sum(i int) i64 {
	res := i64(0)
	for i > 0 {
		res += f.t[i]
		i -= i & -i
	}
	return res
}

var N int = 1e5 + 10

func log(args ...interface{}) {
	fmt.Println(args...)
}

var inf = []int64{0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18}

type Item2 struct {
	value    int
	priority int
}

type LazyPriorityQueue struct {
	items []Item2
	size  int
	mem   map[int]int
}

func (pq *LazyPriorityQueue) Len() int {
	return len(pq.items)
}

// x 小根堆，优先队列, 优先级低的先出
func (pq *LazyPriorityQueue) Less(i, j int) bool {
	return pq.items[i].priority < pq.items[j].priority
}

func (pq *LazyPriorityQueue) Swap(i, j int) {
	pq.items[i], pq.items[j] = pq.items[j], pq.items[i]
}

func (pq *LazyPriorityQueue) Push(x interface{}) {
	item := x.(Item2)
	pq.items = append(pq.items, item)
}

func (pq *LazyPriorityQueue) push(item Item2) {
	heap.Push(pq, item)
	if _, ok := pq.mem[item.value]; !ok {
		pq.mem[item.value] = 0
	}
	pq.size++
}

func (pq *LazyPriorityQueue) Pop() interface{} {
	old := pq.items
	n := len(old)
	item := old[n-1]
	pq.items = old[:n-1]
	return item
}

func (pq *LazyPriorityQueue) pop(val int) {
	pq.mem[val]++
	pq.size--
}

func (pq *LazyPriorityQueue) Top() Item2 {
	pq.ApplyRemove()
	return pq.items[0]
}

func (pq *LazyPriorityQueue) ApplyRemove() {
	for pq.Len() > 0 && pq.mem[pq.items[0].value] > 0 {
		val := pq.items[0].value
		heap.Pop(pq)
		pq.mem[val]--
	}
}

func max(a, b interface{}) interface{} {
	switch a.(type) {
	case int:
		return max(a.(int), b.(int))
	case i64:
		return max(a.(i64), b.(i64))
	case f64:
		return max(a.(f64), b.(f64))
	case u64:
		return max(a.(u64), b.(u64))
	case u32:
		return max(a.(u32), b.(u32))
	default:
		return a
	}
}

func min(a, b interface{}) interface{} {
	switch a.(type) {
	case int:
		return min(a.(int), b.(int))
	case i64:
		return min(a.(i64), b.(i64))
	case f64:
		return min(a.(f64), b.(f64))
	case u64:
		return min(a.(u64), b.(u64))
	case u32:
		return min(a.(u32), b.(u32))
	default:
		return a
	}
}

var dp [509][509]int

var a [505]int

func main() {
	io := NewIO()
	defer io.flush()

	t := int(io.I())
	for ; t > 0; t-- {
		n := int(io.I())
		for i := 1; i <= n; i++ {
			a[i] = int(io.I())
		}
		ans := 0

		for k := 2; k <= 2000; k++ {
			for i := 1; i <= n; i++ {
				for j := 1; j <= n; j++ {
					dp[i][j] = 0
				}
			}

			for i := 1; i <= n; i++ {
				if a[i]*2 == k {
					dp[i][i] = 1
				}
			}

			for length := 2; length <= n; length++ {
				for l := 1; l+length-1 <= n; l++ {
					r := l + length - 1
					dp[l][r] = max(dp[l][r], dp[l+1][r]).(int)

					dp[l][r] = max(dp[l][r], dp[l+1][r-1]).(int)
					if a[l]+a[r] == k {
						dp[l][r] = max(dp[l][r], dp[l+1][r-1]+2).(int)
					}
				}
			}
			ans = max(&ans, dp[1][n]).(int)
		}
		io.print(ans)
	}
}
