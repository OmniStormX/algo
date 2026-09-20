#include <algorithm>
#include <vector>
#include <iostream>
using std::vector;
template <class T>
concept IsVector = requires(T t) {
    []<class U, class A>(const std::vector<U, A>&){}(t);
};

template <class T>
void print(T a) {
	if constexpr (IsVector<T>) {
		if (a.size() == 0) {
			std::cout << "[]";
		} else {
			std::cout << "[";
			print(a[0]);
			for (int i = 1; i < a.size(); i++) {
				print(",");
				print(a[i]);
			}
			std::cout << "]";
		}
	} else {
		std::cout << a;
	}
}

class Solution {
public:
    vector<int> largestPower(vector<int>& nums) {
		vector<vector<int>> seg;
		int n = nums.size();

		seg.push_back(nums);
		int M = 14;
		vector<int> L;
		vector<int> power(15);
		int mx = *std::max_element(nums.begin(), nums.end());
		M = std::__lg(mx);
		while (M >= 0) {
			int pre = 0;
			for (int i = 0; i < seg.size(); i++) {
				vector<int> one, zero;
				for (int x: seg[i]) {
					if (x >> M & 1) {
						one.push_back(x);
					} else {
						zero.push_back(x);
					}
				}
				pre += one.size();
				if (zero.size() == 0) continue;
				if (one.size() > 0) {
					seg[i] = std::move(one);
					seg.insert(seg.begin() + i + 1, zero);
				}
			}
			power[14 - M] = pre;

		}
		return power;
    }
};


int main() {
	Solution s;
	vector<int> a = {1, 4};
	vector<int> ans = s.largestPower(a);

	for (int i = 0; i < ans.size(); i++) {
		std::cout << ans[i] << " \n"[i == ans.size() - 1];
	}
}