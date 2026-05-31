#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <ostream>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::get;
using std::map;
using std::max;
using std::min;
using std::set;
using std::string;
using std::tuple;
using std::unordered_map;
using std::vector;

template <typename T>
std::ostream &operator<<(std::ostream &os, const vector<T> &a) {
  if (!a.size())
    os << "[]";
  else {
    os << "[";
    os << a[0];
    for (int i = 1; i < a.size(); i++) {
      os << ", " << a[i];
    }
    os << "]";
  }
  return os;
}

template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}

template <typename... Args>
std::ostream &operator<<(std::ostream &os, const std::tuple<Args...> &t) {
  os << "(";
  // 打印tuple中的所有元素
  std::apply([&](const Args &...args) { ((os << args << ", "), ...); }, t);
  os << ")";
  return os;
}
template <class S, S (*Op)(S, S), S (*E)(), class F, S (*Mapping)(F, S),
          F (*Composition)(F, F), F (*Id)()>
class LazySegTree {
public:
  LazySegTree() : LazySegTree(0) {}

  explicit LazySegTree(int n) : LazySegTree(vector<S>(n, E())) {}

  explicit LazySegTree(const vector<S> &v) : n_(static_cast<int>(v.size())) {
    size_ = 1;
    log_ = 0;
    while (size_ < n_) {
      size_ <<= 1;
      ++log_;
    }
    data_.assign(size_ << 1, E());
    lazy_.assign(size_, Id());
    for (int i = 0; i < n_; ++i) {
      data_[size_ + i] = v[i];
    }
    for (int i = size_ - 1; i >= 1; --i) {
      Update(i);
    }
  }

  void Set(int p, S x) {
    assert(0 <= p && p < n_);
    p += size_;
    for (int i = log_; i >= 1; --i) {
      Push(p >> i);
    }
    data_[p] = x;
    for (int i = 1; i <= log_; ++i) {
      Update(p >> i);
    }
  }

  S Get(int p) {
    assert(0 <= p && p < n_);
    p += size_;
    for (int i = log_; i >= 1; --i) {
      Push(p >> i);
    }
    return data_[p];
  }

  S Prod(int l, int r) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) {
      return E();
    }

    l += size_;
    r += size_;
    for (int i = log_; i >= 1; --i) {
      if (((l >> i) << i) != l) {
        Push(l >> i);
      }
      if (((r >> i) << i) != r) {
        Push((r - 1) >> i);
      }
    }

    S left_prod = E();
    S right_prod = E();
    while (l < r) {
      if (l & 1) {
        left_prod = Op(left_prod, data_[l++]);
      }
      if (r & 1) {
        right_prod = Op(data_[--r], right_prod);
      }
      l >>= 1;
      r >>= 1;
    }
    return Op(left_prod, right_prod);
  }

  S AllProd() const { return data_[1]; }

  void Apply(int p, F f) {
    assert(0 <= p && p < n_);
    p += size_;
    for (int i = log_; i >= 1; --i) {
      Push(p >> i);
    }
    data_[p] = Mapping(f, data_[p]);
    for (int i = 1; i <= log_; ++i) {
      Update(p >> i);
    }
  }

  void Apply(int l, int r, F f) {
    assert(0 <= l && l <= r && r <= n_);
    if (l == r) {
      return;
    }

    l += size_;
    r += size_;
    for (int i = log_; i >= 1; --i) {
      if (((l >> i) << i) != l) {
        Push(l >> i);
      }
      if (((r >> i) << i) != r) {
        Push((r - 1) >> i);
      }
    }

    int left = l;
    int right = r;
    while (l < r) {
      if (l & 1) {
        AllApply(l++, f);
      }
      if (r & 1) {
        AllApply(--r, f);
      }
      l >>= 1;
      r >>= 1;
    }

    for (int i = 1; i <= log_; ++i) {
      if (((left >> i) << i) != left) {
        Update(left >> i);
      }
      if (((right >> i) << i) != right) {
        Update((right - 1) >> i);
      }
    }
  }

  template <bool (*G)(S)> int MaxRight(int l) {
    return MaxRight(l, [](S x) { return G(x); });
  }

  template <class G> int MaxRight(int l, G g) {
    assert(0 <= l && l <= n_);
    assert(g(E()));
    if (l == n_) {
      return n_;
    }
    l += size_;
    for (int i = log_; i >= 1; --i) {
      Push(l >> i);
    }

    S sm = E();
    do {
      while ((l & 1) == 0) {
        l >>= 1;
      }
      if (!g(Op(sm, data_[l]))) {
        while (l < size_) {
          Push(l);
          l <<= 1;
          if (g(Op(sm, data_[l]))) {
            sm = Op(sm, data_[l]);
            ++l;
          }
        }
        return l - size_;
      }
      sm = Op(sm, data_[l]);
      ++l;
    } while ((l & -l) != l);
    return n_;
  }

  template <bool (*G)(S)> int MinLeft(int r) {
    return MinLeft(r, [](S x) { return G(x); });
  }

  template <class G> int MinLeft(int r, G g) {
    assert(0 <= r && r <= n_);
    assert(g(E()));
    if (r == 0) {
      return 0;
    }
    r += size_;
    for (int i = log_; i >= 1; --i) {
      Push((r - 1) >> i);
    }

    S sm = E();
    do {
      --r;
      while (r > 1 && (r & 1)) {
        r >>= 1;
      }
      if (!g(Op(data_[r], sm))) {
        while (r < size_) {
          Push(r);
          r = r << 1 | 1;
          if (g(Op(data_[r], sm))) {
            sm = Op(data_[r], sm);
            --r;
          }
        }
        return r + 1 - size_;
      }
      sm = Op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

private:
  int n_ = 0;
  int size_ = 1;
  int log_ = 0;
  vector<S> data_;
  vector<F> lazy_;

  void Update(int k) { data_[k] = Op(data_[k << 1], data_[k << 1 | 1]); }

  void AllApply(int k, F f) {
    data_[k] = Mapping(f, data_[k]);
    if (k < size_) {
      lazy_[k] = Composition(f, lazy_[k]);
    }
  }

  void Push(int k) {
    if (lazy_[k] == Id()) {
      return;
    }
    AllApply(k << 1, lazy_[k]);
    AllApply(k << 1 | 1, lazy_[k]);
    lazy_[k] = Id();
  }
};

class Info {
public:
  int x;
  Info(int _x = -1) : x(_x) {}
};

Info E() { return Info(); }

Info Op(Info a, Info b) {
  if (a.x == -1)
    return b.x;
  if (b.x == -1)
    return a.x;
  return std::gcd(a.x, b.x);
}

class Tag {
public:
  bool operator==(const Tag &other) const { return true; }
};

Tag Id() { return Tag(); }
Info Mapping(Tag f, Info x) { return x; }

Tag Composition(Tag f, Tag g) { return g; }

class Solution {
public:
  int countGoodSubseq(vector<int> &nums, int p, vector<vector<int>> &queries) {
    int n = nums.size();
    LazySegTree<Info, Op, E, Tag, Mapping, Composition, Id> seg(n);
    for (int i = 0; i < n; i++) {
      if (nums[i] % p == 0) {
        seg.Set(i, Info(nums[i]));
      }
    }

    int ans = 0;
    int q = queries.size();
    for (int i = 0; i < q; i++) {
      int pos = queries[i][0], x = queries[i][1];
      if (nums[pos] % p == 0) {
        seg.Set(pos, Info(-1));
      }
      nums[pos] = x;
      if (nums[pos] % p == 0) {
        seg.Set(pos, Info(nums[pos]));
      }
      // cerr << seg.AllProd().x << "\n";
      if (seg.AllProd().x == p) {
        if (n > 30) {
          ans++;
        } else {
          if (n == 1)
            continue;
          for (int i = 0; i < n; i++) {
            int x = -1;
            if (i > 0)
              x = seg.Prod(0, i).x;
            if (x == -1) {
              if (i + 1 < n) {
                x = seg.Prod(i + 1, n).x;
              }
            } else {
              if (i + 1 < n) {
                x = std::gcd(seg.Prod(i + 1, n).x, x);
              }
            }
            if (x == p) {
              ans++;
              break;
            }
          }
        }
      }
    }
    return ans;
  }
};

int main() {

  Solution s;
  vector<int> nums = {1, 6};
  vector q = vector<vector<int>>{{0, 7}};
  int ans = s.countGoodSubseq(nums, 7, q);
  cout << ans << "\n";
}