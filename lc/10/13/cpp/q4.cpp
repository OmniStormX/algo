#include <algorithm>
#include <bits/stdc++.h>
#define debug(x)    std::cerr << #x << " = " << x << std::endl
using namespace std;
struct node {
    int ch[10];
    int pos;
    node() {
        memset(ch, 0, sizeof(ch));
        pos = -1;
    }
};
template<typename T>
inline bool chkmax(T &a, T b) {
    if (a < b) {
        a = b;
        return 1;
    }
    return 0;
}

template<typename T>
inline bool chkmin(T &a, T b) {
    if (a > b) {
        a = b;
        return 1;
    }
    return 0;
}

class Solution {
    using i64 = long long;
    
    
public:
    i64 calc(const vector<int>& a, const vector<int>& b) {
        i64 ans = 0;
        int i = 0, j = 0;
        while (i < a.size() && j < b.size()) {
            while (i < a.size() && a[i] <= b[j]) i++;
            ans += a.size() - i;
            j++;
        }
        return ans;
    }
    int minInversions(vector<string>& words, vector<int>& target) {
        vector<node> t;
        t.push_back(node());
        vector<int> pos(target.size());
        for (int i = 0; i < target.size(); i++) pos[target[i]] = i;
        int rt = 0;
        auto add = [&]() {
            t.push_back(node());
            return ++rt;
        };

        int n = words.size();
        for (int i = 0; i < n; i++) {
            int u = 0;
            for (auto c: words[i]) {
                if (t[u].ch[c - 'a'] == 0) {
                    t[u].ch[c - 'a'] = add();
                }
                u = t[u].ch[c - 'a'];
            }
            t[u].pos = pos[i];
        }
        i64 ans = 0;
        auto dfs = [&](auto&& dfs, int u) -> vector<int> {
            
            vector<vector<int>> a;
            for (int i = 0; i < 10; i++) {
                if (t[u].ch[i]) {
                    a.push_back(std::move(dfs(dfs, t[u].ch[i])));
                }
            }
            
            int m = a.size();
            vector<vector<int>> inv(m, vector<int>(m, 0));  
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < m; j++) {
                    if (i == j) continue;
                    inv[i][j] = calc(a[i], a[j]);
                    // a[i] 左侧， a[j] 右侧的逆序对
                }
            }
            
            vector<int> dp((1 << m), 1e9);
            dp[0] = 0;
            
            for (int i = 0; i < (1 << m); i++) {
                int s = i;
                for (int j = 0; j < m; j++) {
                    if (s >> j & 1) continue;
                    int sum = dp[s];
                    for (int k = 0; k < m; k++) {
                        if (s >> k & 1) {
                            sum += inv[k][j];
                        }
                    }
                    chkmin(dp[s | (1 << j)], sum);
                }
            }
            ans += dp[(1 << m) - 1];
            
            for (auto sub: a) {
                if (t[u].pos != -1)
                    ans += std::lower_bound(sub.begin(), sub.end(), t[u].pos) - sub.begin();
            }

            vector<int> ret;
            for (auto ax: a) {
                ret.insert(ret.end(), ax.begin(), ax.end());
            }
            if (t[u].pos != -1) ret.push_back(t[u].pos);
            ranges::sort(ret);
            return ret;
        };

        dfs(dfs, 0);
        return ans;
    }
};

int main() {
    vector<string> words = {"bad","bag","fig","fed"};
    vector<int> target = {3,1,0,2};
    Solution s;
    cout << s.minInversions(words, target);
}