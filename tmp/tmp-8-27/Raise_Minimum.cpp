#include <algorithm>
#include <functional>
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
#include <iomanip>

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

std::ostream& operator<<(std::ostream& os, i128 x) {
    if (x == 0) {
        return os << '0';
    }
    if (x < 0) {
        os << '-';
        x = -x;
    }
    char s[50];
    int len = 0;
    while (x > 0) {
        s[len++] = char('0' + x % 10);
        x /= 10;
    }
    while (len--) {
        os << s[len];
    }
    return os;
}

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

using Z = ModInt<1000000007>;


struct Point {
    int x;
    int y;

    Point() = default;

    bool operator< (const Point& rhs) const {
        return y < rhs.y || (y == rhs.y && x < rhs.x);
    }
};


class Solution {

public:
    void solve() {
        int n;
        i64 k;
        cin >> n >> k;
        std::vector<i64> A(n);
        for (int i = 0; i < n; i++) cin >> A[i];


        i128 l = *min_element(A.begin(), A.end()), r = 1E24;

        auto check = [&](i128 ave) {
            i128 cnt = 0;
            for (int i = 0; i < n; i++) {
                if (A[i] < ave) {
                    i128 sum = ave - A[i];
                    if (sum % (i + 1) == 0) {
                        cnt += sum / (i + 1);
                    } else {
                        cnt += sum / (i + 1) + 1;
                    }
                }
            }
            return cnt <= k;
        };

        while (l < r - 1) {
            i128 mid = (l + r) / 2;
            if (check(mid)) {
                l = mid;
            } else {
                r = mid;
            }
        }

        cout << l << "\n";

    }
};


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    Solution s;
    int t = 1;
    while (t--) {
        s.solve();
    }
}
