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
    int n, L;
    cin >> n >> L;
    vector<int> x(n);
    // return;
    for (int i = 0; i < n; i++) cin >> x[i];
    for (int i = 0; i < n - 1; i++) {
        x.push_back(x[i] + L);
    }
    sort(x.begin(), x.end());
    i64 ans = 1LL * n * (n - 1) * (n - 2) / 6;
    int l = 0;
    // double half = L / 2.0;
    i64 U = 0;
    int r = 0;
    for (int l = 0; l < n; l++) {
        // debug(r);
        while (r + 1 < l + n && L > (x[r + 1] - x[l]) * 2LL ) r++;
        if (r - l + 1 >= 3) {
            i64 k = r - l;
            U += k * (k - 1) / 2;
        }
    }
    cout << ans - U << "\n";
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
