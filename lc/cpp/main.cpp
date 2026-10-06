#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>
#include <type_traits>
#include <utility>
using std::mutex;

using std::function;
class ThreadPool {


public:
	ThreadPool(int n = std::thread::hardware_concurrency()): n_(n), running(true) {
		workers.reserve(n);
		for (int i = 0; i < n_; i++) {
			workers.push_back(std::jthread([this]() {
				workloop();
			}));
		}
	}

	template<class F, class ... Args>
	std::future<std::invoke_result_t<F&&, Args&&...>> submit(F&& f, Args&&... args) {
		using R = std::invoke_result_t<F&&, Args&&...>;
		using task_R = std::packaged_task<R()>;
		auto task = std::make_shared<task_R>(
			[f = std::forward<F>(f), ...args = std::forward<Args>(args)]() mutable {
				return std::invoke(std::move(f), std::move(args)...);
			}
		);

		auto future = task->get_future();

		{
			std::lock_guard<mutex> lk(mu);
			if (!running) {
				throw std::runtime_error("thread pool stopped.");
			}
			tasks.push([task]() {
				(*task)();
			});
		}

		cv.notify_one();
		return future;
	}

	~ThreadPool() {
		stop();
		cv.notify_all();
	}

	void stop() {
		{
			std::lock_guard<mutex> lk(mu);
			running = false;
		}
	};

private:

	void workloop() {
		while (true) {
			std::function<void()> task;
			{
				std::unique_lock lk(mu);

				cv.wait(lk, [this]() {
					return !running || !tasks.empty();
				});

				if (!running && tasks.empty())
					return;
				task = tasks.front();
				tasks.pop();
			}
			task();
		}
	}

	int n_;
	bool running;
	std::queue<std::function<void()>> tasks;

	std::vector<std::jthread> workers;
	mutex mu;
	std::condition_variable cv;
};


int main()
{
    ThreadPool pool(4);

    auto f1 = pool.submit([] {
        return 1 + 2;
    });

    auto f2 = pool.submit([](int a, int b) {
        return a * b;
    }, 6, 7);

    pool.submit([] {
        std::cout << "hello thread pool\n";
    });

    std::cout << f1.get() << '\n'; // 3
    std::cout << f2.get() << '\n'; // 42
}