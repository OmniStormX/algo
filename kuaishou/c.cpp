#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <utility>
#include <vector>
// #define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
#define int long long

using i64 = int64_t;

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




signed main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;
    s--;
    vector<int> w(n);
    graph g(n);
    for (int i = 0; i < n; i++) {
        cin >> w[i];
    }
    i64 ans = 0;

    vector<vector<int>> e(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        g.add_edge(u, v);
        e[u].push_back(v);
    }
    for (int i = 0; i < n; i++) {
        if (g.dfn[i] == 0) g.tarjan(i);
    }
    vector<i64> d(n + 1, 0);
    vector<vector<int>> ee(g.get_scc_cnt() + 1);
    for (int i = 0; i < n; i++) {
        // debug(i);
        d[g.get_scc_id(i)] += w[i];
        for (auto v: e[i]) {
            if (g.get_scc_id(i) != g.get_scc_id(v))
                ee[g.get_scc_id(i)].push_back(g.get_scc_id(v));
        }
    }

    std::function<i64(int)> dfs = [&](int u)-> i64 {
        // debug(u);
        i64 ret = 0;
        for (auto v: ee[u]) {
            ret = max(ret, dfs(v));
        }
        return ret + d[u];
    };

    cout << dfs(g.get_scc_id(s)) << "\n";
}