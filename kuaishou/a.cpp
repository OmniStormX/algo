/*
题目大意：有一个阵列横向排列，小兵占一格，炮兵占两格，不允许两个炮兵占一起。
求长度为 n 的阵列有多少种排列方式，按 1E9 + 7 取模。

*/ 

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

using i64 = int64_t;
constexpr int M = 1e9 + 7;

constexpr int L = 4;
struct matrix {
    int a[4][4];

    matrix(int x = 0) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                a[i][j] = 0;
        if (x == 1) {
            for (int i = 0; i < L; i++) a[i][i] = 1;
        }
    }

    matrix operator*(const matrix& b) const& {
        matrix ans;
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < L; j++) {
                for (int k = 0; k < L; k++) {
                    ans.a[i][k] = (ans.a[i][k] + i64(a[i][j]) * b.a[j][k] % M) % M;
                }
            }
        }
        return ans;
    }
};

// #define debug(x)    std::cerr << #x << " = " << x << std::endl

void print(matrix m) {
    std::cerr << "[";
    for (int i = 0; i < 4; i++) {
        std::cerr << "[";
        for (int j = 0; j < 4; j++)
            std::cerr << m.a[i][j] << " ";
        std::cerr << "]\n";
    }
    std::cerr << "]" << std::endl;
}

int calc(int n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    matrix c;
    c.a[0][0] = 1;
    c.a[0][1] = 0;
    c.a[0][2] = 1;
    c.a[0][3] = 1;

    matrix s;
    // dp_{i - 2}_0, dp_{i - 2}_1, dp_{i - 1}_0, dp_{i - 1}_1, 
    // dp_i_0 = dp_{i - 1}_0 + dp_{i - 1}_1
    // dp_i_1 = dp_{i - 2}_0
    s.a[2][0] = 1;
    s.a[3][1] = 1;
    s.a[2][2] = 1;
    s.a[3][2] = 1;
    s.a[0][3] = 1;
    int y = n - 2;
    matrix d(1);
    while (y > 0) {
        if (y & 1) d = d * s;
        y >>= 1;
        s = s * s;
    }
    c = c * d;
    return (c.a[0][3] + c.a[0][2]) % M;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    i64 n;
    cin >> n;
    cout << calc(n) << "\n";
    

}
// 64 位输出请用 printf("%lld")