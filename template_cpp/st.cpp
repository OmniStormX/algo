

#include <cassert>
#include <vector>

#include <bits/stdc++.h>
using namespace std;

template <
    typename Info,
    typename Merge
>
class STTable {
private:
    int n_ = 0;
    int log_ = 0;

    vector<vector<Info>> st_;
    Merge merge_;

    inline static vector<int> lg2_ = {0, 0};

    static void init_log(int n) {
        if ((int)lg2_.size() > n)
            return;

        int old = lg2_.size();

        lg2_.resize(n + 1);

        for (int i = max(2, old); i <= n; i++) {
            lg2_[i] = lg2_[i >> 1] + 1;
        }
    }

public:
    STTable() = default;

    explicit STTable(
        const vector<Info>& a,
        Merge merge = Merge()
    ) : merge_(merge) {
        build(a);
    }

    void build(const vector<Info>& a) {
        n_ = a.size();

        if (n_ == 0) {
            st_.clear();
            log_ = 0;
            return;
        }

        init_log(n_);

        log_ = lg2_[n_];

        st_.assign(
            log_ + 1,
            vector<Info>(n_)
        );

        // 第 0 层：长度 1
        st_[0] = a;

        // st[k][i]
        // 表示 [i, i + 2^k - 1]
        for (int k = 1; k <= log_; k++) {
            int len = 1 << k;
            int half = len >> 1;

            for (int i = 0; i + len <= n_; i++) {
                st_[k][i] = merge_(
                    st_[k - 1][i],
                    st_[k - 1][i + half]
                );
            }
        }
    }

    // 查询闭区间 [l, r]
    Info query(int l, int r) const {
        assert(0 <= l && l <= r && r < n_);

        int len = r - l + 1;
        int k = lg2_[len];

        return merge_(
            st_[k][l],
            st_[k][r - (1 << k) + 1]
        );
    }

    int size() const {
        return n_;
    }
};