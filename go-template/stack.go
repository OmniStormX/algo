package gotemplate

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
