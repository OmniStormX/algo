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

constexpr int M = 1000000007;

void solve() {
    string K;
    int D;
    cin >> K >> D;
    vector dp(10000, vector<int>(100, -1));

    auto calc = [&](const string& K, int p, int D, bool isLimit, bool isNum, int sum) -> int {
        if (isNum)
    };
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
