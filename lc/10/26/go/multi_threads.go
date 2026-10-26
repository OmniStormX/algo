package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

const testTimeout = 5 * time.Second

// ==================== Blocking Queue ====================

type BoundedQueue struct {
	mu    sync.Mutex
	cond  *sync.Cond
	queue *Queue[int]
}

func NewBoundedQueue(capacity int) *BoundedQueue {
	q := &BoundedQueue{
		queue: newQueue[int](capacity),
	}

	q.cond = sync.NewCond(&q.mu)
	return q
}

// Block while the queue is full.
func (q *BoundedQueue) Enqueue(x int) {
	q.mu.Lock()
	defer q.mu.Unlock()

	for q.queue.size == q.queue.cap {
		q.cond.Wait()
	}

	q.queue.Push(x)

	// The queue state changed. Wake waiting producers/consumers.
	q.cond.Broadcast()
}

// Block while the queue is empty.
func (q *BoundedQueue) Dequeue() int {
	q.mu.Lock()
	defer q.mu.Unlock()

	for q.queue.size == 0 {
		q.cond.Wait()
	}

	x, err := q.queue.Pop()
	if err != nil {
		panic("queue unexpectedly empty")
	}

	q.cond.Broadcast()
	return x
}

// Return the current queue length safely.
func (q *BoundedQueue) Size() int {
	q.mu.Lock()
	defer q.mu.Unlock()

	return q.queue.size
}

// ==================== Test Helpers ====================

func must(condition bool, message string) {
	if !condition {
		panic("FAIL: " + message)
	}
}

func await(done <-chan struct{}, message string) {
	timer := time.NewTimer(testTimeout)
	defer timer.Stop()

	select {
	case <-done:
	case <-timer.C:
		panic("TIMEOUT: " + message)
	}
}

func receiveInt(ch <-chan int, message string) int {
	timer := time.NewTimer(testTimeout)
	defer timer.Stop()

	select {
	case v := <-ch:
		return v
	case <-timer.C:
		panic("TIMEOUT: " + message)
	}
}

// ==================== Test 1: FIFO ====================

func testBasicOperations() {
	fmt.Println("[TEST] Basic FIFO operations")

	q := NewBoundedQueue(3)

	must(q.Size() == 0, "initial size must be zero")

	q.Enqueue(10)
	q.Enqueue(20)
	q.Enqueue(30)

	must(q.Size() == 3, "size must be three")

	must(q.Dequeue() == 10, "FIFO: expected 10")
	must(q.Dequeue() == 20, "FIFO: expected 20")
	must(q.Dequeue() == 30, "FIFO: expected 30")

	must(q.Size() == 0, "size must return to zero")

	fmt.Println("  PASS")
}

// ==================== Test 2: Producer Blocking ====================

func testProducerBlocksWhenFull() {
	fmt.Println("[TEST] Producer blocks when queue is full")

	q := NewBoundedQueue(1)
	q.Enqueue(1)

	done := make(chan struct{})

	go func() {
		defer close(done)
		q.Enqueue(2)
	}()

	// Enqueue(2) must not finish while the queue is full.
	select {
	case <-done:
		panic("FAIL: producer did not block on a full queue")
	case <-time.After(100 * time.Millisecond):
	}

	must(q.Dequeue() == 1, "expected first item to be 1")

	// Removing an item must allow the producer to proceed.
	await(done, "producer remained blocked after space became available")

	must(q.Size() == 1, "queue should contain the second item")
	must(q.Dequeue() == 2, "expected second item to be 2")

	fmt.Println("  PASS")
}

// ==================== Test 3: Consumer Blocking ====================

func testConsumerBlocksWhenEmpty() {
	fmt.Println("[TEST] Consumer blocks when queue is empty")

	q := NewBoundedQueue(1)
	result := make(chan int, 1)

	go func() {
		result <- q.Dequeue()
	}()

	// Dequeue must not finish while the queue is empty.
	select {
	case v := <-result:
		panic(fmt.Sprintf(
			"FAIL: consumer returned %d from an empty queue", v,
		))
	case <-time.After(100 * time.Millisecond):
	}

	q.Enqueue(42)

	v := receiveInt(result, "consumer remained blocked after enqueue")
	must(v == 42, "consumer should receive 42")

	fmt.Println("  PASS")
}

// ==================== Test 4: Concurrent Producers/Consumers ====================

func testConcurrentProducersAndConsumers() {
	fmt.Println("[TEST] Concurrent producers and consumers")

	const (
		capacity         = 7
		producerCount    = 4
		consumerCount    = 5
		itemsPerProducer = 1000
		total            = producerCount * itemsPerProducer
		stopValue        = -1
	)

	q := NewBoundedQueue(capacity)

	var produced atomic.Int64
	var consumed atomic.Int64

	// seen[v] records how many times value v was consumed.
	seen := make([]atomic.Int32, total)

	var producers sync.WaitGroup
	var consumers sync.WaitGroup

	// Start consumers first. They may block on an empty queue.
	consumers.Add(consumerCount)

	for i := 0; i < consumerCount; i++ {
		go func() {
			defer consumers.Done()

			for {
				v := q.Dequeue()

				// Each consumer exits after receiving one sentinel.
				if v == stopValue {
					return
				}

				if v < 0 || v >= total {
					panic(fmt.Sprintf("invalid item: %d", v))
				}

				if seen[v].Add(1) != 1 {
					panic(fmt.Sprintf("duplicate item: %d", v))
				}

				consumed.Add(1)
			}
		}()
	}

	// Start producers. Each produces a unique range of integers.
	producers.Add(producerCount)

	for p := 0; p < producerCount; p++ {
		go func(id int) {
			defer producers.Done()

			start := id * itemsPerProducer
			end := start + itemsPerProducer

			for v := start; v < end; v++ {
				q.Enqueue(v)
				produced.Add(1)
			}
		}(p)
	}

	// Wait for all producers, with a timeout.
	producerDone := make(chan struct{})

	go func() {
		producers.Wait()
		close(producerDone)
	}()

	await(producerDone, "producers did not finish")

	must(
		produced.Load() == int64(total),
		"wrong produced count",
	)

	// All producers have finished.
	// FIFO guarantees all normal values precede these sentinels.
	for i := 0; i < consumerCount; i++ {
		q.Enqueue(stopValue)
	}

	// Wait for all consumers to exit, with a timeout.
	consumerDone := make(chan struct{})

	go func() {
		consumers.Wait()
		close(consumerDone)
	}()

	await(consumerDone, "consumers did not finish")

	// Validate the results after all consumers have exited.
	must(
		consumed.Load() == int64(total),
		"wrong consumed count",
	)

	for i := 0; i < total; i++ {
		must(
			seen[i].Load() == 1,
			fmt.Sprintf("missing or duplicate item: %d", i),
		)
	}

	must(q.Size() == 0, "queue must be empty")

	fmt.Println("  PASS")
}

// ==================== Main ====================

func main() {
	testBasicOperations()
	testProducerBlocksWhenFull()
	testConsumerBlocksWhenEmpty()
	testConcurrentProducersAndConsumers()

	fmt.Println("All tests passed.")
}
