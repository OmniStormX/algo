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
#include <format>
#include <cassert>

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
using std::vector;
using std::queue;

using i64 = int64_t;
using i128 = __int128_t; 

#define println(...) std::cerr << std::format(__VA_ARGS__) << endl

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

constexpr int M = 998244353;

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

vector<int> order;
vector<int> pos(200009);
int C[200009];

struct ACAutomaton {
    constexpr static int L = 26;

    struct node {
        int fail;
        int cnt;
        int son[L];
        node() {
            fail = 0;
            cnt = 0;
            memset(son, 0, sizeof(son));
        }
    };
    vector<node> trie;
    int idx;
    ACAutomaton(int n = 0) {
        trie.assign(n, node());
        idx = 0;
        trie.push_back(node());
    }

    int newNode() {
        assert(idx + 1 < trie.size());
        return ++idx;
    }

    void insert(const string& s, int id) {
        int cur = 0;
        for (char c : s) {
            int i = c - 'a';
            if (trie[cur].son[i] == 0) {
                trie[cur].son[i] = newNode();
            }
            cur = trie[cur].son[i];
        }
        pos[id] = cur;
    }

    void build() {
        queue<int> q;
        for (int i = 0; i < L; i++) {
            int v = trie[0].son[i];
            if (v) {
                trie[v].fail = 0;
                q.push(v);
            }
        }
        while (q.size()) {
            int u = q.front();
            order.push_back(u);
            q.pop();
            for (int i = 0; i < L; i++) {
                int v = trie[u].son[i];
                if (v) {
                    q.push(v);
                    trie[v].fail = trie[trie[u].fail].son[i];
                } else {
                    trie[u].son[i] = trie[trie[u].fail].son[i];
                }
            }
        }
    }

    void query(const string& s) {
        int u = 0;
        for (auto c : s) {
            int i = c - 'a';
            u = trie[u].son[i];
            trie[u].cnt++;
        }
        
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int u = order[i];
            trie[trie[u].fail].cnt += trie[u].cnt;
        }
    }
};


struct matrix {
    vector<vector<Z>> a;

    matrix(int n, int m) : a(n, vector<Z>(m, 0)) {}

    void set(int x, int y, Z val) {
        a[x][y] = val;
    }

    matrix operator * (const matrix& rhs) const {
        int n = a.size();
        int m = a[0].size();
        int k = rhs.a[0].size();
        
        matrix result(n, k);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < k; j++) {
                for (int l = 0; l < m; l++) {
                    result.a[i][j] += a[i][l] * rhs.a[l][j];
                }
            }
        }
        
        return result;
    }

    matrix operator ^ (int n) const {

        matrix result(a.size(), a[0].size());
        for (int i = 0; i < a.size(); i++) {
            result.a[i][i] = 1;
        }
        matrix base = *this;
        while (n > 0) {
            if (n & 1) result = result * base;
            base = base * base;
            n >>= 1;
        }
        return result;
    }

    Z get(int x, int y) const {
        return a[x][y];
    }

};
ACAutomaton ac(200009);

void solve() {
    int n;
    cin >> n;
    vector<string> match(n);
    for (int i = 0; i < n; i++) {
        cin >> match[i];
        ac.insert(match[i], i);
    }

    ac.build();
    string S;
    cin >> S;
    ac.query(S);
    for (int i = 0; i < n; i++) {
        cout << ac.trie[pos[i]].cnt << "\n";
    }
    
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
