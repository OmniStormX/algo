// https://leetcode.cn/problems/count-good-strings/
class Solution {
public:
    using i64 = long long;
    static constexpr int M = 1e9 + 7;

    struct matrix {
        int x[2][2];

        matrix(int _x = 1) {
            x[0][0] = _x;
            x[1][1] = _x;
            x[0][1] = x[1][0] = 0;
        };

        matrix operator*(const matrix& b) const & {
            matrix ans(0);
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        ans.x[i][j] += i64(x[i][k]) * b.x[k][j] % M;
                        ans.x[i][j] %= M;
                    }
                }
            }
            return ans;
        };
    };

    matrix qpow(matrix x, i64 y) {
        matrix ans = matrix(1);
        while (y > 0) {
            if (y & 1) ans = ans * x;
            y >>= 1;
            x = x * x;
        }
        return ans;
    }

    int countGoodStrings(long long n) {
        if (n == 1) return 2;
        if (n == 2) return 2;
        i64 k = n - 2;

        matrix c(0);
        c.x[0][1] = c.x[1][0] = c.x[1][1] = 1;
        matrix d(0);
        d.x[0][0] = d.x[0][1] = 1;
        c = qpow(c, n - 2);
        d = d * c;
        return i64(d.x[0][1]) * 2 % M;
    }
};