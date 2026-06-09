#include <algorithm>
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>
#include <set>
#include <cstring>
#include <map>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <cassert>
#include <string>
#include <cmath>
using namespace std;

#define debug(x) cerr << #x << " = " << x << endl

constexpr double pi = acos(-1);
using i64 = int64_t;

void solve() {
    int n, m;
    cin >> n >> m;
    
    vector e(n, vector<int>());
    vector<int> in(n, 0);
    vector<int> dp(n, 0);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        // e[u].push_back(v);
        e[v].push_back(u);
        in[u]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (in[i] == 0) {
            q.push(i);
            // dp[i] = 1;
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < e[u].size(); i++) {
            int v = e[u][i];
            in[v]--;
            if (in[v] == 0) {
                q.push(v);
            }
            dp[v] = max(dp[u] + 1, dp[v]);
        }
    }

    cout << *std::max_element(dp.begin(), dp.end()) << "\n";
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}
