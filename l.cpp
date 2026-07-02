#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <cmath>
#include <assert.h>
#include <functional>
#include <map>
#include <tuple>
#include <format>
#include <deque>
#include <queue>
#include <span>

#define println(...)    std::cerr << std::format(__VA_ARGS__) << std::endl
#define dbg(x)  std::cerr << __LINE__ << ":" << #x << " = " << x << std::endl

using i64 = long long;
using i128 = __int128_t;

template<typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (T i : v) {
        os << i << ", ";
    }
    os << "]";
    return os;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const std::span<T>& v) {
    os << "[";
    for (T i : v) {
        os << i << ", ";
    }
    os << "]";
    return os;
}

template<typename T, typename U>
std::ostream& operator<< (std::ostream& os, const std::pair<T, U>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

template<typename... Args>
std::ostream& operator<< (std::ostream& os, const std::tuple<Args...>& t) {
    os << "(";
    std::apply([&](const Args&... args) { ((os << args << ", "), ...); }, t);
    os << ")";
    return os;
}

void write(i128 x) {
    if (x > 9) {
        write(x / 10);
    }
    std::cout << (char)(x % 10 + '0');
}

std::ostream& operator << (std::ostream& os, i128 x) {
    if (x < 0) {
        os << "-";
        x = -x;
    }
    write(x);
    return os;
}

std::istream& operator >> (std::istream& is, i128& x) {
    x = 0;
    i128 f = 1;
    while (!isdigit(is.peek())) {
        if (is.peek() == '-') {
            f = -1;
        }
        is.get();
    }
    while (isdigit(is.peek())) {
        x = x * 10 + is.get() - '0';
    }
    x *= f;
    return is;
}

namespace matrix {
    constexpr int Mod = 1E9 + 7;
    struct matrix {
        int h, w;
        std::vector<std::vector<int>> a;
    
        matrix(int h, int w) : h(h), w(w), a(h, std::vector<int>(w, 0)) 
        {}
    
        friend matrix operator* (const matrix& a, const matrix& b) {
            matrix ans(a.h, a.w);
            assert(a.w == b.h);
            for (int i = 0; i < a.h; i++) {
                for (int j = 0; j < a.w; j++) {
                    if (a.a[i][j] == 0) {
                            continue;
                        }
                    for (int k = 0; k < b.w; k++) {
                        ans.a[i][k] = (ans.a[i][k] + 1LL * a.a[i][j] * b.a[j][k] % Mod) % Mod;
                        // if (ans.a[i][k] > Mod) ans.a[i][k] %= Mod;
                    }
                }
            }
            return ans;
        }
    };
}

class BipartiteGraphic {
public:
    struct edge {
        int v;
        int cost;
        edge(int v = 0, int cost = 0) : v(v), cost(cost)
        {
        }
    };

    std::vector<std::vector<edge>> e;
    std::vector<int> color;
    std::vector<bool> vis;

    BipartiteGraphic(int n) : e(n + 1, std::vector<edge>())
    {
    }

    void addEdge(int u, int v, int cost = 0) {
        e[u].push_back({v, cost});
        e[v].push_back({u, cost});
    }

    void initDinic() {
        color = std::vector<int>(e.size(), 0);
    }

    bool dinic(int u, int col) {
        // dbg(u);
        if (color[u] == col) return false;
        color[u] = col;
        for (auto [v, c] : e[u]) {

            if (color[v] == 0 || dinic(v, col)) {
                color[v] = col;
                return 1;
            }
        }
        return false;
    }
};

 
template<const int T>
struct ModInt {
    const static int mod = T;
    int x;
    ModInt(int x = 0) : x(x % mod) {}
    ModInt(long long x) : x(int(x % mod)) {} 
    int val() { return x; }
    ModInt operator + (const ModInt &a) const { int x0 = x + a.x; return ModInt(x0 < mod ? x0 : x0 - mod); }
    ModInt operator - (const ModInt &a) const { int x0 = x - a.x; return ModInt(x0 < 0 ? x0 + mod : x0); }
    ModInt operator * (const ModInt &a) const { return ModInt(1LL * x * a.x % mod); }
    ModInt operator / (const ModInt &a) const { return *this * a.inv(); }
    bool operator == (const ModInt &a) const { return x == a.x; };
    bool operator != (const ModInt &a) const { return x != a.x; };
    void operator += (const ModInt &a) { x += a.x; if (x >= mod) x -= mod; }
    void operator -= (const ModInt &a) { x -= a.x; if (x < 0) x += mod; }
    void operator *= (const ModInt &a) { x = 1LL * x * a.x % mod; }
    void operator /= (const ModInt &a) { *this = *this / a; }
    friend ModInt operator + (int y, const ModInt &a){ int x0 = y + a.x; return ModInt(x0 < mod ? x0 : x0 - mod); }
    friend ModInt operator - (int y, const ModInt &a){ int x0 = y - a.x; return ModInt(x0 < 0 ? x0 + mod : x0); }
    friend ModInt operator * (int y, const ModInt &a){ return ModInt(1LL * y * a.x % mod);}
    friend ModInt operator / (int y, const ModInt &a){ return ModInt(y) / a;}
    friend std::ostream &operator<<(std::ostream &os, const ModInt &a) { return os << a.x;}
    friend std::istream &operator>>(std::istream &is, ModInt &t){return is >> t.x;}
    
    ModInt pow(int64_t n) const {
        ModInt res(1), mul(x);
        while(n){
            if (n & 1) res *= mul;
            mul *= mul;
            n >>= 1;
        }
        return res;
    }
        
    ModInt inv() const {
        int a = x, b = mod, u = 1, v = 0;
        while (b) {
            int t = a / b;
            a -= t * b; std::swap(a, b);
            u -= t * v; std::swap(u, v);
        }
        if (u < 0) u += mod;
        return u;
    }
        
};

using Z = ModInt<998244353>;

template<class T = int>
class Fenwicktree {
    std::vector<T> t;
    std::vector<int> tag;
    int time = 0;
public :
    Fenwicktree(int n): t(n + 1, 0), tag(n + 1, 0) {}
 
   inline int lowbit(int x) { return x & -x; }
 
   void add(int x, T y) {
       while (x < t.size()) {
            if (tag[x] == time)
                t[x] += y;
            else {
                tag[x] = time;
                t[x] = y;
            }
            x += lowbit(x);
       }
   }
 
   T quiry(int x) {
       T ans = 0;
       while (x) ans += (tag[x] == time ? t[x]: 0), x -= lowbit(x);
       return ans;
   }

    void clear() { time++; }
 
   T quiry(int l, int r) {
       assert(l > 0);
       return quiry(r) - quiry(l - 1);
   }
};

class HeuristicDsu {
public:
    std::vector<int> t, size;
    HeuristicDsu(int n): t(n), size(n) {
        for (int i = 0; i < n; i++) t[i] = i, size[i] = 1;

    }

    int find(int x) {
        return t[x] == x ? x : find(t[x]);
    }
    
    void merge(int x, int y) {
        int k1 = find(x), k2 = find(y);
        if (k1 == k2) return;
        if (size[k1] > size[k2]) std::swap(k1, k2);
        t[k1] = k2;
        size[k2] += size[k1];
    }
};


template<class Mint>
struct Combination {
    int n;
    std::vector<Mint> fac, ifac, inv;

    Combination(int n = 0) {
        init(n);
    }

    void init(int m) {
        if (m <= n) return;

        fac.resize(m + 1);
        ifac.resize(m + 1);
        inv.resize(m + 1);

        if (n == 0) {
            fac[0] = ifac[0] = 1;
            if (m >= 1) inv[1] = 1;
        }

        for (int i = std::max(1, n + 1); i <= m; i++) {
            fac[i] = fac[i - 1] * i;
        }

        ifac[m] = fac[m].inv();
        for (int i = m; i > n; i--) {
            ifac[i - 1] = ifac[i] * i;
        }

        for (int i = std::max(2, n + 1); i <= m; i++) {
            inv[i] = ifac[i] * fac[i - 1];
        }

        n = m;
    }

    Mint C(int n, int k) {
        if (k < 0 || k > n) return 0;
        init(n);
        return fac[n] * ifac[k] * ifac[n - k];
    }

    Mint A(int n, int k) {
        if (k < 0 || k > n) return 0;
        init(n);
        return fac[n] * ifac[n - k];
    }

    Mint H(int n, int k) {
        // multiset combination: C(n + k - 1, k)
        if (n < 0 || k < 0) return 0;
        if (n == 0) return k == 0 ? 1 : 0;
        return C(n + k - 1, k);
    }

    Mint fact(int n) {
        if (n < 0) return 0;
        init(n);
        return fac[n];
    }

    Mint inv_fact(int n) {
        if (n < 0) return 0;
        init(n);
        return ifac[n];
    }

    Mint inverse(int n) {
        if (n <= 0) return 0;
        init(n);
        return inv[n];
    }
};
using std::pair;using std::vector;using std::cout;using std::cin;using std::string;using std::min;
using std::max;using std::cerr;using std::function;using std::unordered_map;using std::map;
using std::set;using std::deque;using std::swap;using std::queue;


class Aho_Corasick {

    struct node {
        int son[26];
        int fail;
        int val;
        node() {
            fail = -1;
            memset(son, 0, sizeof(son));
            val = 0;
        }
    };
    vector<node> t;
    int cur;
    const static L = 26;
public:
    Aho_Corasick(int n): t(n, node()), cur(0) {
        t[0].fail = 0;
    }

    int newNode() {
        assert(cur + 1 < t.size());
        return ++cur;
    }

    // 插入一个字符串，返回插入恰好以当前字符串为后缀的状态的节点的位置。
    int insert(const string& s, int id) {
        int u = 0;
        for (auto c: s) {
            int p = c - 'a';
            if (t[u].son[p] == 0) {
                t[u].son[p] = newNode();
            }
            u = t[u].son[p];
        }
        t[p].val++;
        return u;
    }


    void build() {
        queue<int> q;
        for (int i = 0; i < L; i++) {
            int v = t[0].son[i];
            if (v) {
                t[v].fail = 0;
                q.push(v);
            }
        }
        while (q.size()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < L; i++) {
                int v = t[u].son[i];
                if (v) {
                    q.push(v);
                    t[v].fail = t[t[u].fail].son[i];
                } else {
                    t[u].son[i] = t[t[u].fail].son[i];
                }
            }
        }
    }


};

void solve() {
	int n, m;
    cin >> n >> m;
    string s;
    Aho_Corasick ac(500);
    for (int i = 0; i < n; i++) {
        cin >> s;
        ac.insert(s, i);
    }

    ac.build();

    for (int i = 0; i < m; i++) {
        string text;
        cin >> text;

    }
}

signed main() {
    std::ios::sync_with_stdio(false);
    cin.tie(nullptr), std::cout.tie(nullptr);
    int t = 1;
    while (t--) {
        solve();
	}
    return 0;
}