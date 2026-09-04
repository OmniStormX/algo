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

using std::pair;
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::map;
using std::unordered_map;
using std::tuple;
using std::get;
using std::max;
using std::min;

using i64 = int64_t;
using i128 = __int128_t;

#define debug(x)  std::cerr << #x << " = " << x << std::endl

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& a) {
    if (!a.size()) os << "[]";
    else {
        os << "[";
        os << a[0];
        for (size_t i = 1; i < a.size(); i++) {
            os << ", " << a[i];
        }
        os << "]";
    }
    return os;
}

template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}


template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";

    // 使用 first 标记控制分隔符，避免首元素前多打印逗号。
    bool first = true;
    std::apply(
        [&](const auto&... args) {
            ((os << (first ? "" : ", ") << args, first = false), ...);
        },
        t);

    os << ")";
    return os;
}

constexpr int M = 1e9 + 7;

template <int MOD>
struct ModInt {
    int v;

    ModInt() : v(0) {}

    template <class T>
    ModInt(T x) {
        long long y = (long long)x % MOD;
        if (y < 0) y += MOD;
        v = (int)y;
    }

    static int mod() {
        return MOD;
    }

    explicit operator int() const {
        return v;
    }

    ModInt operator + () const {
        return *this;
    }

    ModInt operator - () const {
        return ModInt(v ? MOD - v : 0);
    }

    ModInt& operator += (const ModInt& rhs) {
        v += rhs.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }

    ModInt& operator -= (const ModInt& rhs) {
        v -= rhs.v;
        if (v < 0) v += MOD;
        return *this;
    }

    ModInt& operator *= (const ModInt& rhs) {
        v = (long long)v * rhs.v % MOD;
        return *this;
    }

    ModInt& operator /= (const ModInt& rhs) {
        return *this *= rhs.inv();
    }

    friend ModInt operator + (ModInt lhs, const ModInt& rhs) {
        return lhs += rhs;
    }

    friend ModInt operator - (ModInt lhs, const ModInt& rhs) {
        return lhs -= rhs;
    }

    friend ModInt operator * (ModInt lhs, const ModInt& rhs) {
        return lhs *= rhs;
    }

    friend ModInt operator / (ModInt lhs, const ModInt& rhs) {
        return lhs /= rhs;
    }

    bool operator == (const ModInt& rhs) const {
        return v == rhs.v;
    }

    bool operator != (const ModInt& rhs) const {
        return v != rhs.v;
    }

    ModInt pow(long long n) const {
        ModInt a = *this;
        ModInt res = 1;
        while (n > 0) {
            if (n & 1) res *= a;
            a *= a;
            n >>= 1;
        }
        return res;
    }

    ModInt inv() const {
        // MOD 必须是质数，且 v != 0
        return pow(MOD - 2);
    }

    friend std::ostream& operator << (std::ostream& os, const ModInt& x) {
        return os << x.v;
    }

    friend std::istream& operator >> (std::istream& is, ModInt& x) {
        long long y;
        is >> y;
        x = ModInt(y);
        return is;
    }
};

using Z = ModInt<M>;

class AC_automation {
	static constexpr int L = 26;
	struct node {
		int son[L];
		int fail;
		int val;
		int depth;
		int id;
		node() {
			memset(son, 0, sizeof(son));
			fail = val = id = 0;
		}
	};
	std::vector<node> t;
	int cur;
public:
	AC_automation(int n = 1) {
		t.assign(n, node());
		cur = 0;
	}

	int insert(const string& s, int id) {
		int u = 0;
		for (char c: s) {
			int p = c - 'a';
			int v = t[u].son[p];
			if (!v) {
				v = t[u].son[p] = ++cur;
			}
			u = v;
		}
		t[u].id = id;
		t[u].val = (1 << t[u].depth);
		return u;
	}

	void build() {
		std::queue<int> q;
		std::vector<int> ord;
		ord.push_back(0);
		t[0].depth = 0;
		for (int i = 0; i < L; i++) {
			int v = t[0].son[i];
			if (v) {
				t[v].fail = 0;
				q.push(v);
				t[v].depth = 1;
				if (t[v].id) {
					t[v].val = (1 << t[v].depth);
				}
			}
		}

		while (!q.empty()) {
			int u = q.front();
			ord.push_back(u);
			q.pop();
			for (int i = 0; i < L; i++) {
				int v = t[u].son[i];
				if (v) {
					t[v].fail = t[t[u].fail].son[i];
					t[v].depth = t[u].depth + 1;
					if (t[v].id) t[v].val = (1 << t[v].depth);
					q.push(v);
				} else {
					t[u].son[i] = t[t[u].fail].son[i];
				}
			}
		}

		for (int v: ord) {
			t[v].val |= t[t[v].fail].val;
		}
	}


	int query(const string& s) {
		int u = 0;
		int dp = 1;
		int n = s.size();
// 		0, 1, 2, 3, 4, ......, 20
		int mx = 0;
		for (int i = 0; i < s.size(); i++) {
			int co = s[i] - 'a';
			dp <<= 1;
			u = t[u].son[co];
			if (dp & (t[u].val)) {
				dp |= (1 << t[u].depth) | 1;
				mx = i + 1;
				// dbg(mx);
			}
		}
		return mx;
	}
};

void solve() {
	string s, t;
	cin >> s >> t;
	int n = s.size(), m = t.size();

	std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			dp[i + 1][j + 1] = max(dp[i + 1][j], dp[i][j + 1]);
			if (s[i] == t[j]) {
				dp[i + 1][j + 1] = max({dp[i][j] + 1, dp[i + 1][j + 1]});
			}
			// std::cerr << dp[i + 1][j + 1] << " \n"[j == m - 1];
		}
	}

	string ans = "";

	int x = n, y = m;
	while (true) {
		if (dp[x][y] == 0) break;
		if (y > 0 && dp[x][y - 1] == dp[x][y]) y--;
		else if (x > 0 && dp[x - 1][y] == dp[x][y]) x--;
		else if (x > 0 && y > 0 && dp[x - 1][y - 1] + 1 == dp[x][y]) {
			x--, y--;
			// cerr << x << ", " << y << "\n";
			ans += s[x];
		}
	}
	std::reverse(ans.begin(), ans.end());
	cout << ans << "\n";
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int t = 1;
    while (t--) {
        solve();
    }
	return 0;
}
