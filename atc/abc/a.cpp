#include <bits/stdc++.h>
using std::cin;
using std::cout;
using std::vector;

void solve() {
	int n, m, k;
	cin >> n >> m >> k;

	std::vector<vector<int>> e(n);
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		u--;
		v--;
		e[u].push_back(v);
		e[v].push_back(u);
	}
	std::set<int> ban;

	for (int i = 0; i < k; i++) {
		int x;
		cin >> x;
		ban.insert(x - 1);
	}
	std::queue<int> q;
	q.push(0);
	vector<int> d(n, 1e9);
	vector<bool> vis(n, false);
	// d[0] = 0;
	d[0] = 0;
	int step = 0;
	while (!q.empty()) {
		int L = q.size();
		for (int i = 0; i < L; i++) {
			int u = q.front();
			q.pop();
			if (vis[u]) {
				continue;
			}
			vis[u] = 1;
			for (auto v: e[u]) if(ban.count(v) == 0) {
				if (d[v] > d[u] + 1) {
					d[v] = d[u] + 1;
					q.push(v);
				}
			}
		}
	}

	if (d[n - 1] == 1e9) d[n - 1] = -1;
	cout << d[n - 1] << "\n";
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