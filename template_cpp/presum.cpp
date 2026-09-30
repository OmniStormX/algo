#include <vector>
using std::vector;

template<typename T = int>
class preSum {

	vector<T> t;
public:
// 	0-base
	preSum(const vector<T>&a): t(a.size() + 1){
		for (int i = 0; i < a.size(); i++) {
			t[i] = t[i - 1] + a[i - 1];
		}
	}

	// 0-base
	// [l, r]
	T Q(int l, int r) {
		return t[r + 1] - t[l];
	}
};