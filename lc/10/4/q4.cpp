// https://leetcode.cn/problems/count-good-strings/
// #pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>


#include <cstdint>
#include <iostream>

template <std::uint32_t mod_, bool fast = false>
struct MontgomeryModInt {
 private:
  using mint = MontgomeryModInt;
  using i32 = std::int32_t;
  using i64 = std::int64_t;
  using u32 = std::uint32_t;
  using u64 = std::uint64_t;

  static constexpr u32 get_r() {
    u32 ret = mod_;
    for (i32 i = 0; i < 4; i++) ret *= 2 - mod_ * ret;
    return ret;
  }

  static constexpr u32 r = get_r();

  static constexpr u32 n2 = -u64(mod_) % mod_;

  static_assert(r * mod_ == 1, "invalid, r * mod != 1");
  static_assert(mod_ < (1 << 30), "invalid, mod >= 2 ^ 30");
  static_assert((mod_ & 1) == 1, "invalid, mod % 2 == 0");

  u32 x;

 public:
  MontgomeryModInt() : x{} {}

  MontgomeryModInt(const i64& a)
      : x(reduce(u64(fast ? a : (a % mod() + mod())) * n2)) {}

  static constexpr u32 reduce(const u64& b) {
    return u32(b >> 32) + mod() - u32((u64(u32(b) * r) * mod()) >> 32);
  }

  mint& operator+=(const mint& p) {
    if (i32(x += p.x - 2 * mod()) < 0) x += 2 * mod();
    return *this;
  }

  mint& operator-=(const mint& p) {
    if (i32(x -= p.x) < 0) x += 2 * mod();
    return *this;
  }

  mint& operator*=(const mint& p) {
    x = reduce(u64(x) * p.x);
    return *this;
  }

  mint& operator/=(const mint& p) {
    *this *= p.inv();
    return *this;
  }

  mint operator-() const { return mint() - *this; }

  mint operator+(const mint& p) const { return mint(*this) += p; }

  mint operator-(const mint& p) const { return mint(*this) -= p; }

  mint operator*(const mint& p) const { return mint(*this) *= p; }

  mint operator/(const mint& p) const { return mint(*this) /= p; }

  bool operator==(const mint& p) const {
    return (x >= mod() ? x - mod() : x) == (p.x >= mod() ? p.x - mod() : p.x);
  }

  bool operator!=(const mint& p) const {
    return (x >= mod() ? x - mod() : x) != (p.x >= mod() ? p.x - mod() : p.x);
  }

  u32 val() const {
    u32 ret = reduce(x);
    return ret >= mod() ? ret - mod() : ret;
  }

  mint pow(u64 n) const {
    mint ret(1), mul(*this);
    while (n > 0) {
      if (n & 1) ret *= mul;
      mul *= mul;
      n >>= 1;
    }
    return ret;
  }

  mint inv() const { return pow(mod() - 2); }

  friend std::ostream& operator<<(std::ostream& os, const mint& p) {
    return os << p.val();
  }

  friend std::istream& operator>>(std::istream& is, mint& a) {
    i64 t;
    is >> t;
    a = mint(t);
    return is;
  }

  static constexpr u32 mod() { return mod_; }
};

template <std::uint32_t mod>
using modint = MontgomeryModInt<mod>;
using modint998244353 = modint<998244353>;
using modint1000000007 = modint<1000000007>;

template <class T>
struct Matrix {
  std::vector<std::vector<T> > A;

  Matrix() {}

  Matrix(const std::vector<std::vector<T> >& A) : A(A) {}

  Matrix(std::size_t n, std::size_t m) : A(n, std::vector<T>(m, 0)) {}

  Matrix(std::size_t n) : A(n, std::vector<T>(n, 0)) {};

  std::size_t size() const {
    if (A.empty()) return 0;
    assert(A.size() == A[0].size());
    return A.size();
  }

  std::size_t height() const { return (A.size()); }

  std::size_t width() const { return (A[0].size()); }

  inline const std::vector<T>& operator[](int k) const { return (A.at(k)); }

  inline std::vector<T>& operator[](int k) { return (A.at(k)); }

  static Matrix I(std::size_t n) {
    Matrix mat(n);
    for (int i = 0; i < n; i++) mat[i][i] = 1;
    return (mat);
  }

  Matrix& operator+=(const Matrix& B) {
    std::size_t n = height(), m = width();
    assert(n == B.height() && m == B.width());
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) (*this)[i][j] += B[i][j];
    return (*this);
  }

  Matrix& operator-=(const Matrix& B) {
    std::size_t n = height(), m = width();
    assert(n == B.height() && m == B.width());
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) (*this)[i][j] -= B[i][j];
    return (*this);
  }

  Matrix& operator*=(const Matrix& B) {
    std::size_t n = height(), m = B.width(), p = width();
    assert(p == B.height());
    std::vector<std::vector<T> > C(n, std::vector<T>(m, 0));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
        for (int k = 0; k < p; k++)
          C[i][j] = (C[i][j] + (*this)[i][k] * B[k][j]);
    A.swap(C);
    return (*this);
  }

  Matrix& operator^=(long long k) {
    Matrix B = Matrix::I(height());
    while (k > 0) {
      if (k & 1) B *= *this;
      *this *= *this;
      k >>= 1LL;
    }
    A.swap(B.A);
    return (*this);
  }

  Matrix operator+(const Matrix& B) const { return (Matrix(*this) += B); }

  Matrix operator-(const Matrix& B) const { return (Matrix(*this) -= B); }

  Matrix operator*(const Matrix& B) const { return (Matrix(*this) *= B); }

  Matrix operator^(const long long k) const { return (Matrix(*this) ^= k); }

  friend std::ostream& operator<<(std::ostream& os, Matrix& p) {
    std::size_t n = p.height(), m = p.width();
    for (int i = 0; i < n; i++) {
      os << "[";
      for (int j = 0; j < m; j++) {
        os << p[i][j] << (j + 1 == m ? "]\n" : ",");
      }
    }
    return (os);
  }

  T determinant() {
    Matrix B(*this);
    assert(width() == height());
    T ret = 1;
    for (int i = 0; i < width(); i++) {
      int idx = -1;
      for (int j = i; j < width(); j++) {
        if (B[j][i] != 0) idx = j;
      }
      if (idx == -1) return (0);
      if (i != idx) {
        ret *= -1;
        std::swap(B[i], B[idx]);
      }
      ret *= B[i][i];
      T vv = B[i][i];
      for (int j = 0; j < width(); j++) {
        B[i][j] /= vv;
      }
      for (int j = i + 1; j < width(); j++) {
        T a = B[j][i];
        for (int k = 0; k < width(); k++) {
          B[j][k] -= B[i][k] * a;
        }
      }
    }
    return (ret);
  }
};


class Solution {
public:


    int countGoodStrings(long long n) {
        if (n == 1) return 2;
        if (n == 2) return 2;

        Matrix<modint1000000007> c(2), d(2);
        c[0][1] = c[1][0] = c[1][1] = 1;
        d[0][0] = d[0][1] = 1;
        c ^= (n - 2);
        d *= c;
        return (d[0][1] * 2).val();
    }
};

int main() {
	Solution s;
	int x;
	std::cin >> x;
	std::cout << s.countGoodStrings(x) << std::endl;
}