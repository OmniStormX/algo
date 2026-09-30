#include <bits/stdc++.h>
#include <cstddef>
using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::set;

void solve() {
	int n, q;
	cin >> n >> q;

	string ans(n, 'a');
	set<int> s0, s1;
	vector<std::pair<int, string>> query(q);
	vector<int> cur(n, 0);
	for (int i = 0; i < q; i++) {
		cin >> query[i].first >> query[i].second;
		if (query[i].first == 1) {
			int x = stoi(query[i].second) - 1;
			cur[x] ^= 1;
		}
	}

	for (int i = 0; i < n; i++) {
		if (cur[i] == 0)
			s1.insert(i);
	}

	vector<bool> covered(n, false);

	for (int i = q - 1; i >= 0; i--) {
		int op = query[i].first;
		if (op == 1) {
			int x = stoi(query[i].second) - 1;
			cur[x] ^= 1;
			if (cur[x] == 0 && covered[x] == false) {
				s1.insert(x);
			}
			if (cur[x] == 1 && s1.count(x) > 0) {
				s1.erase(x);
			}
		} else {
			for (auto x: s1) {
				ans[x] = query[i].second[0];
				covered[x] = true;
			}
			s1.clear();
		}
	}
	cout << ans << "\n";
}

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int t;
	// cin >> t;
	t = 1;
	while (t--)
		solve();
	return 0;
}