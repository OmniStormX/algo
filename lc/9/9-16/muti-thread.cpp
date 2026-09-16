// https://leetcode.cn/problems/fizz-buzz-multithreaded/

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
class FizzBuzz {
private:
    int n;
    std::atomic<int> cur;
    std::mutex mu;
    std::condition_variable cv;
public:
    FizzBuzz(int n) {
        this->n = n;
        this->cur = 1;
    }

    // printFizz() outputs "fizz".
    void fizz(std::function<void()> printFizz) {
        for (int i = 3; i <= n; i += 3) {
            if (i % 5 == 0)
                continue;
            {
                std::unique_lock lock(mu);
                cv.wait(lock, [this, i]() {
                    return this->cur == i;
                });
                printFizz();
                ++cur;
            }
            cv.notify_all();
        }
    }

    // printBuzz() outputs "buzz".
    void buzz(std::function<void()> printBuzz) {
        for (int i = 5; i <= n; i += 5) {
            if (i % 3 == 0)
                continue;
            {
                std::unique_lock lock(mu);
                cv.wait(lock, [this, i]() {
                    return this->cur == i;
                });
                printBuzz();
                ++cur;
            }
            cv.notify_all();
        }
    }

    // printFizzBuzz() outputs "fizzbuzz".
	void fizzbuzz(std::function<void()> printFizzBuzz) {
        for (int i = 15; i <= n; i += 15) {
            {
                std::unique_lock lock(mu);
                cv.wait(lock, [this, i]() {
                    return this->cur == i;
                });
                printFizzBuzz();
                ++cur;
            }
            cv.notify_all();
        }
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(std::function<void(int)> printNumber) {
        for (int i = 1; i <= n; i++) {
            if (i % 3 == 0 or i % 5 == 0)
                continue;
            {
                std::unique_lock lock(mu);
                cv.wait(lock, [this, i]() {
                    return this->cur == i;
                });
                printNumber(cur);
                ++cur;
            }
            cv.notify_all();
        }
    }
};