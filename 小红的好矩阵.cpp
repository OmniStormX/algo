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


struct Point {
    int x;
    int y;

    Point() = default;

    bool operator< (const Point& rhs) const {
        return y < rhs.y || (y == rhs.y && x < rhs.x);
    }
};

void solve() {
    int n, m, k;
    cin >> n >> m >> k;

    std::set<Point> s;
    
    for (int i = 0; i < k; i++) {
        Point p;
        cin >> p.x >> p.y;
        s.insert(std::move(p));
    }

    Z up = n, down = 0;
    int row = 1;
    int up_l = n, down_l = 0;
    if (m == 1) {
        cout << (k == 0 ? 1 : 0) << "\n";
        return;
    }
    Z ans = 0;
    for (auto [x, y]: s) {
        if (y == 1) {
            up = x - 1;
            up_l = x - 1;
            down_l = 0;
            down = 0;
            continue;
        }
        int k = y - row;
        if (k > 1) {
            Z sum = up + down;
            sum = sum * Z(n).pow(k - 1);
            down = 0;
            up = sum;
            up_l = n, down_l = 0;
        }
        if (y == m) {
            if (up_l > x) {
                ans += up / up_l * (up_l - x);
            }
            if (down_l > 0) {
                ans += down / down_l * min(down_l, n - x);
            }
            row = y;
            continue;
        }
        Z up_t = 0, down_t = 0;
        int up_l_t = x - 1, down_l_t = n - x;
        if (up.v > 0) {
            up_t = up / up_l * min(up_l, x - 1);
        }
        if (down_l > n - x + 1) {
            up_t += down / down_l * (down_l - n + x - 1);
        }

        if (down.v > 0) down_t = down / down_l * min(down_l, n - x);
        if (up.v > 0 && up_l > x) down_t += up / up_l * (up_l - x);
        down_l_t = n - x;
        up = up_t * up_l_t;
        down = down_t * down_l_t;
        up_l = up_l_t;
        down_l = down_l_t;
        row = y;
    }
    if (row < m) {
        int k = m - row;
        if (k > 1) {
            Z sum = up + down;
            sum = sum * Z(n).pow(k - 1);
            down = 0;
            up = sum;
            up_l = n, down_l = 0;
        }
        ans = up + down;
    }
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
}
