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

using std::vector;
using std::set;
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

template<typename T>
std::ostream& operator<<(std::ostream& os, const vector<T>& a) {
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

vector<int> getFac(int x) {
    vector<int> fac;
    for (int i = 1; 1LL * i * i <= x; i++) {
        if (x % i == 0) {
            fac.push_back(i);
            if (i != x / i) fac.push_back(x / i);
        }
    }
    return fac;
}

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> l(n), r(n);
    for (int i = 0; i < n; i++) cin >> l[i] >> r[i];

    vector<int> Z;
    
    for (int i = 0; i < n; i++) {
        Z.push_back(l[i]);
        Z.push_back(r[i]);
        Z.push_back(l[i] - 1);
    }

    Z.push_back(1E9 + 1);
    std::sort(Z.begin(), Z.end());
    Z.erase(std::unique(Z.begin(), Z.end()), Z.end());
    int N = Z.size();
    vector<int> c(N, 0);

    for(int i = 0; i < n; i++) {
        int l1 = std::lower_bound(Z.begin(), Z.end(), l[i]) - Z.begin();
        int r1 = std::lower_bound(Z.begin(), Z.end(), r[i]) - Z.begin() + 1;

        c[l1]++;
        c[r1]--;
    }

    cout << *std::max_element(c.begin(), c.end()) << "\n";
}   

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int t = 1;
    while (t--) {
        solve();
    }
}
