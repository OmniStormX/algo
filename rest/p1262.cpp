#include <bits/stdc++.h>
#include <climits>
#include <cstdint>
#include <unordered_map>
using namespace std;

using i64 = long long;

class graph {

public:
	vector<vector<int>> g;
	vector<int> dfn, low;
	vector<int> stk;
	vector<int> in_stk;
	vector<int> scc_id;

	int timer = 0;
	int scc_cnt = 0;

	graph(int n = 0): g(n), dfn(n), low(n), in_stk(n), scc_id(n) {}

	// Add an edge from u to v
	void add_edge(int u, int v) {
		g[u].push_back(v);
	}

	bool is_visited(int u) {
		return dfn[u] != 0;
	}

	int get_scc_id(int u) {
		return scc_id[u];
	}

	int get_scc_cnt() {
		return scc_cnt;
	}

	void tarjan(int u) {
		dfn[u] = low[u] = ++timer;

		stk.push_back(u);
		in_stk[u] = 1;

		for (int v : g[u]) {
			if (!dfn[v]) {
				// 树边
				tarjan(v);

				low[u] = min(low[u], low[v]);
			} else if (in_stk[v]) {
				// 指向当前尚未确定 SCC 的点
				low[u] = min(low[u], dfn[v]);
			}
		}

		// u 是一个 SCC 的根
		if (dfn[u] == low[u]) {
			++scc_cnt;

			while (true) {
				int v = stk.back();
				stk.pop_back();

				in_stk[v] = 0;
				scc_id[v] = scc_cnt;

				if (v == u)
					break;
			}
		}
	}
};


void solve() {
	int n, p;
	cin >> n >> p;
	const int inf = 1e9;
	vector<int> a(n, inf);

	for (int i = 0; i < p; i++) {
		int u, c;
		cin >> u >> c;
		u--;
		a[u] = c;
	}

	int m;
	cin >> m;
	graph g(n);
	for (int i = 0 ; i < m; i++) {
		int a, b;
		cin >> a >> b;
		a--, b--;
		g.add_edge(a, b);
	}

	int ans = inf;
	for (int i = 0; i < n; i++) if (!g.is_visited(i)) {
		g.tarjan(i);
	}
	int N = g.get_scc_cnt();
	vector<vector<int>> e(N + 1);
	vector<int> A(N + 1, inf);
	vector<bool> vis(N + 1, false);
	vector<int> in(N + 1, 0);
	for (int i = 0; i < n; i++) {
		int id = g.get_scc_id(i);
		A[id] = min(A[id], a[i]);
		for (auto v: g.g[i]) {
			int idv = g.get_scc_id(v);
			if (idv != id) {
				e[id].push_back(idv);
				in[idv]++;
			}
		}
	}

	auto dfs = [&](auto&& self, int u) -> void {
		vis[u] = true;

		for (auto v : e[u]) {
			if (!vis[v]) {
				self(self, v);
			}
		}
	};

	for (int i = 1; i <= N; i++) {
		if (A[i] < inf && !vis[i]) {
			dfs(dfs, i);
		}
	}
	i64 sum = 0;
	for (int i = 0; i < n; i++) {
		int id = g.get_scc_id(i);
		if (!vis[id]) {
			cout << "NO\n";
			cout << i + 1 << "\n";
			return ;
		}
	}


	for (int i = 1; i <= N; i++) {
		if (in[i] == 0) sum += A[i];
	}
	cout << "YES\n";
	cout << sum << "\n";
}


int main() {
    std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int t = 1;
	// cin >> t;
	while (t--)
		solve();

	return 0;
}