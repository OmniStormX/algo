#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

template <class S, S (*Op)(S, S), S (*E)(), class F, S (*Mapping)(F, S),
          F (*Composition)(F, F), F (*Id)()>
class LazySegTree {
 public:
  LazySegTree() : LazySegTree(0) {}

  explicit LazySegTree(int n) : LazySegTree(vector<S>(n, E())) {}

  explicit LazySegTree(const vector<S>& v) : n_(static_cast<int>(v.size())) {
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

  template <bool (*G)(S)>
  int MaxRight(int l) {
    return MaxRight(l, [](S x) { return G(x); });
  }

  template <class G>
  int MaxRight(int l, G g) {
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

  template <bool (*G)(S)>
  int MinLeft(int r) {
    return MinLeft(r, [](S x) { return G(x); });
  }

  template <class G>
  int MinLeft(int r, G g) {
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

// 下面给出一个常见例子:
// 1. 维护区间和。
// 2. 支持区间加。
struct Node {
  long long sum = 0;
  int len = 0;
};

struct Tag {
  long long add = 0;

  bool operator==(const Tag& other) const { return add == other.add; }
  bool operator!=(const Tag& other) const { return !(*this == other); }
};

Node Op(Node a, Node b) { return {a.sum + b.sum, a.len + b.len}; }

Node E() { return {0, 0}; }

Node Mapping(Tag f, Node x) {
  x.sum += f.add * x.len;
  return x;
}

Tag Composition(Tag f, Tag g) {
  // 先作用旧标记 g，再作用新标记 f。
  return {f.add + g.add};
}

Tag Id() { return {0}; }

int main() {
  std::ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  vector<Node> init(n);
  for (int i = 0; i < n; ++i) {
    long long x;
    cin >> x;
    init[i] = {x, 1};
  }

  LazySegTree<Node, Op, E, Tag, Mapping, Composition, Id> seg(init);

  // 示例约定:
  // 1 l r x: 对 [l, r) 做区间加 x
  // 2 l r  : 查询 [l, r) 的区间和
  while (q--) {
    int type;
    cin >> type;
    if (type == 1) {
      int l, r;
      long long x;
      cin >> l >> r >> x;
      seg.Apply(l, r, {x});
    } else {
      int l, r;
      cin >> l >> r;
      cout << seg.Prod(l, r).sum << '\n';
    }
  }
  return 0;
}
