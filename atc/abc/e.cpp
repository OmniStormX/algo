#include <algorithm>
#include <bits/stdc++.h>
#define debug(x)	std::cerr << #x << " = " << x << std::endl

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::set;

using i64 = long long;
using std::min;
using std::max;

void solve() {
	int n, q;
	cin >> n >> q;
	vector<int> a(n);
	vector<int> b(n);
	for (int i = 0; i < n; i++)
		cin >> a[i];
	for (int i = 0; i < n; i++)
		cin >> b[i];

	vector<i64> dp(n, 1e18);

	dp[0] = b[0];

	int x = std::min_element(b.begin(), b.end()) - b.begin();
	for (int i = x; i < n * 2 + x; i++) {
		dp[i % n] = min(dp[i % n], min(dp[(i - 1) % n] + a[(i - 1) % n], i64(b[i % n])));
	}

	for (int i = x + 2 * n - 1; i >= x; i--) {
		dp[i % n] = min(dp[i % n], dp[(i + 1) % n] + a[i % n]);
	}


	i64 sum = 0;
	vector<i64> pre(n + 1, 0);
	for (int i = 1; i <= n; i++) {
		pre[i] = pre[i - 1] + a[i - 1];
	}

	auto Q = [&](int l, int r) {
		return pre[r + 1] - pre[l];
	};
	for (int i = 0; i < q; i++) {
		int s, t;
		cin >> s >> t;
		s--;
		t--;
		if (s < n && t < n) {
			i64 ans = dp[s] + dp[t];
			ans = min(ans, Q(s, t - 1));
			ans = min(ans, pre[n] - Q(s, t - 1));
			cout << ans << "\n";
		} else {
			if (s == n) {
				cout << dp[t] << "\n";
			}
			else {
				cout << dp[s] << "\n";
			}
		}
	}


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