#include <bits/stdc++.h>
#include <unordered_map>
using namespace std;

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


int main() {
    std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	std::cout.tie(nullptr);

	int n, m;
	cin >> n >> m;
	graph g(n);
	vector<int> a(n);
	for (int i = 0; i < n; i++) cin >> a[i];
	for (int i = 0; i < m; i++) {
		int u, v;
		cin >> u >> v;
		u--, v--;
		g.add_edge(u, v);
	}
	for (int i = 0; i < n; i++) {
		if (!g.is_visited(i)) {
			g.tarjan(i);
		}
	}

	int N = g.get_scc_cnt();
	std::vector<vector<int>> e(N + 1);

	vector<int> A(N + 1, 0);
	for (int i = 0; i < n; i++) {
		int id = g.get_scc_id(i);

		A[id] += a[i];

		for (int v : g.g[i]) {
			int vid = g.get_scc_id(v);

			if (id != vid) {
				e[id].push_back(vid);
			}
		}
	}

	vector<int> dp(N + 1, -1);
	auto dfs = [&](auto& self, int u) ->int {
		if (dp[u] != -1) {
			return dp[u];
		}
		int ans = 0;
		for (auto v: e[u]) {
			ans = max(ans, self(self, v));
		}
		dp[u] = ans + A[u];
		return ans + A[u];
	};
	int ans = 0;
	for (int i = 1; i <= N; i++) {
		ans = max(ans, dfs(dfs, i));
	}
	cout << ans;


	return 0;
}