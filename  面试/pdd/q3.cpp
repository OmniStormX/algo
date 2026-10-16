#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n, c;
    cin >> n >> c;
    vector<pair<int, int>> p(n);
    vector<int> z;
    for (int i = 0; i < n; i++) {
        cin >> p[i].first >> p[i].second;
        z.push_back(p[i].first);
        z.push_back(p[i].first - 1);
        z.push_back(p[i].second);
        z.push_back(p[i].second - 1);
    }
    std::sort(z.begin(), z.end());
    z.erase(std::unique(z.begin(), z.end()), z.end());
    auto Z = [&](int x) {
        return lower_bound(z.begin(), z.end(), x) - z.begin();
    };

    int N = z.size();
    vector sq(N + 1, std::vector<int>(N + 1, 0));
        // sq = [[0 for __ in range(N + 1)] for _ in range(N + 1)]
    for (auto [x, y]: p) {
        sq[Z(x) + 1][Z(y) + 1] += 1;
    }
        // for x, y in p:
        //     sq[Z(x) + 1][Z(y) + 1] += 1
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            sq[i][j] += sq[i - 1][j] + sq[i][j - 1] - sq[i - 1][j - 1];
        }
    }
    vector<int> zi(N, -1);
    auto getSquare = [&](int x1, int y1, int x2, int y2) {
        return sq[x2][y2] - sq[x1 - 1][y2] - sq[x2][y1 - 1] + sq[x1 - 1][y1 - 1];
    };

    auto check = [&](int x) {
        for (int i = 0; i < N; i++) {
            zi[i] = Z(z[i] + x);
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (getSquare(i, j, zi[i - 1], zi[j - 1]) >= c) {
                    return true;
                }
            }
        }
        return false;
    };

    int l = 0, r = 1000000001;
    while (l < r - 1) {
        int m = (l + r) >> 1;
        if (check(m)) {
            r = m;
        } else {
            l = m;
        }
    }
    cout << r << "\n";


}
// 64 位输出请用 printf("%lld")