#include <bits/stdc++.h>
using namespace std;

using ull = uint64_t;
using u32 = uint32_t;
using i64 = long long;

static constexpr u32 MOD1 = 1000000007;
static constexpr u32 MOD2 = 1000000009;
static constexpr u32 BASE = 911382323;

ull pack_hash(u32 a, u32 b) {
    return (ull(a) << 32) | b;
}

u32 h1(ull x) {
    return x >> 32;
}

u32 h2(ull x) {
    return (u32)x;
}

struct Candidate {
    int idx;            // first string
    int zero;           // leading zeros of first string

    int t;

    // substring of G[t]
    int l1, r1;

    // substring of G[t+1]
    int l2, r2;

    int len;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    vector<string> S(N);
    for (auto &s : S) cin >> s;

    // K == 1 can be handled directly.
    if (K == 1) {
        string ans = "0";

        for (auto s : S) {
            int p = 0;
            while (p < (int)s.size() && s[p] == '0')
                ++p;

            string cur = (p == (int)s.size() ? "0" : s.substr(p));

            if (cur.size() > ans.size() ||
                (cur.size() == ans.size() && cur > ans)) {
                ans = cur;
            }
        }

        cout << ans << '\n';
        return 0;
    }

    // Largest-number ordering.
    auto cmp = [&](const string &a, const string &b) {
        int n = a.size();
        int m = b.size();

        for (int i = 0; i < n + m; ++i) {
            char x = (i < n ? a[i] : b[i - n]);
            char y = (i < m ? b[i] : a[i - m]);

            if (x != y)
                return x > y;
        }

        // a+b == b+a.
        // Any order is valid; add deterministic tie-breaking.
        if (a.size() != b.size())
            return a.size() > b.size();

        return a > b;
    };

    sort(S.begin(), S.end(), cmp);

    vector<int> len(N), zero(N);

    int total_len = 0;

    for (int i = 0; i < N; ++i) {
        len[i] = S[i].size();
        total_len += len[i];

        int z = 0;
        while (z < len[i] && S[i][z] == '0')
            ++z;

        zero[i] = z;
    }

    // positions of strings with exact length L.
    vector<vector<int>> pos(11);

    for (int i = 0; i < N; ++i)
        pos[len[i]].push_back(i);

    /*
        G[t] = concatenation of strings whose length >= t.

        off[t][i] = number of chars contributed to G[t]
                    by S[0 ... i-1].
    */
    vector<vector<int>> off(12, vector<int>(N + 1));

    vector<string> G(12);

    for (int t = 1; t <= 10; ++t) {
        for (int i = 0; i < N; ++i) {
            off[t][i + 1] =
                off[t][i] + (len[i] >= t ? len[i] : 0);
        }

        G[t].reserve(off[t][N]);

        for (int i = 0; i < N; ++i)
            if (len[i] >= t)
                G[t] += S[i];
    }

    // G[11] is empty.
    G[11] = "";

    // powers
    vector<u32> pw1(total_len + 1), pw2(total_len + 1);

    pw1[0] = pw2[0] = 1;

    for (int i = 1; i <= total_len; ++i) {
        pw1[i] = (ull)pw1[i - 1] * BASE % MOD1;
        pw2[i] = (ull)pw2[i - 1] * BASE % MOD2;
    }

    // hash[t] = prefix hash of G[t].
    vector<vector<ull>> H(12);

    for (int t = 1; t <= 11; ++t) {
        H[t].resize(G[t].size() + 1);

        u32 a = 0, b = 0;

        H[t][0] = pack_hash(0, 0);

        for (int i = 0; i < (int)G[t].size(); ++i) {
            int v = G[t][i] - '0' + 1;

            a = ((ull)a * BASE + v) % MOD1;
            b = ((ull)b * BASE + v) % MOD2;

            H[t][i + 1] = pack_hash(a, b);
        }
    }

    auto sub_hash = [&](int t, int l, int r) -> ull {
        int d = r - l;

        u32 a =
            (h1(H[t][r]) -
             (ull)h1(H[t][l]) * pw1[d] % MOD1 +
             MOD1) %
            MOD1;

        u32 b =
            (h2(H[t][r]) -
             (ull)h2(H[t][l]) * pw2[d] % MOD2 +
             MOD2) %
            MOD2;

        return pack_hash(a, b);
    };

    auto append_hash = [&](ull x, ull y, int ylen) -> ull {
        u32 a =
            ((ull)h1(x) * pw1[ylen] + h1(y)) % MOD1;

        u32 b =
            ((ull)h2(x) * pw2[ylen] + h2(y)) % MOD2;

        return pack_hash(a, b);
    };

    auto core_prefix_hash = [&](const Candidate &c, int take) {
        u32 a = 0, b = 0;

        int st = c.zero;

        for (int j = 0; j < take; ++j) {
            int v = S[c.idx][st + j] - '0' + 1;

            a = ((ull)a * BASE + v) % MOD1;
            b = ((ull)b * BASE + v) % MOD2;
        }

        return pack_hash(a, b);
    };

    auto prefix_hash = [&](const Candidate &c, int need) {
        ull res = pack_hash(0, 0);

        int core_len = len[c.idx] - c.zero;

        // core
        int take = min(need, core_len);

        if (take) {
            ull x = core_prefix_hash(c, take);
            res = append_hash(res, x, take);
            need -= take;
        }

        if (!need)
            return res;

        // first filtered range
        int sz1 = c.r1 - c.l1;

        take = min(need, sz1);

        if (take) {
            ull x = sub_hash(c.t, c.l1, c.l1 + take);
            res = append_hash(res, x, take);
            need -= take;
        }

        if (!need)
            return res;

        // second filtered range
        int sz2 = c.r2 - c.l2;

        take = min(need, sz2);

        if (take) {
            ull x =
                sub_hash(c.t + 1, c.l2, c.l2 + take);

            res = append_hash(res, x, take);
        }

        return res;
    };

    auto get_char = [&](const Candidate &c, int p) {
        int core_len = len[c.idx] - c.zero;

        if (p < core_len)
            return S[c.idx][c.zero + p];

        p -= core_len;

        int sz1 = c.r1 - c.l1;

        if (p < sz1)
            return G[c.t][c.l1 + p];

        p -= sz1;

        return G[c.t + 1][c.l2 + p];
    };

    auto better = [&](const Candidate &a,
                      const Candidate &b) {
        if (a.len != b.len)
            return a.len > b.len;

        int L = a.len;

        int lo = 0, hi = L;

        // LCP by hashing.
        while (lo < hi) {
            int mid = (lo + hi + 1) >> 1;

            if (prefix_hash(a, mid) ==
                prefix_hash(b, mid))
                lo = mid;
            else
                hi = mid - 1;
        }

        if (lo == L)
            return false;

        return get_char(a, lo) > get_char(b, lo);
    };

    const int q = K - 1;

    array<int, 11> cnt{};
    cnt.fill(0);

    bool have = false;
    Candidate best{};

    /*
        Scan from right to left.
        cnt[L] contains number of length-L strings after i.
    */
    for (int i = N - 1; i >= 0; --i) {
        int suffix_cnt = N - i - 1;

        // S[i] must contain a non-zero digit.
        if (suffix_cnt >= q && zero[i] < len[i]) {
            int rem = q;

            int t = -1;
            int r = -1;

            i64 suffix_len = 0;

            for (int L = 10; L >= 1; --L) {
                if (rem > cnt[L]) {
                    suffix_len += 1LL * cnt[L] * L;
                    rem -= cnt[L];
                } else {
                    t = L;
                    r = rem;

                    suffix_len += 1LL * r * L;
                    break;
                }
            }

            /*
                Find the r-th length-t string after i.
            */
            auto &v = pos[t];

            int at =
                upper_bound(v.begin(), v.end(), i) -
                v.begin();

            int p = v[at + r - 1];

            Candidate cur;

            cur.idx = i;
            cur.zero = zero[i];
            cur.t = t;

            /*
                (i,p] : all len >= t
            */
            cur.l1 = off[t][i + 1];
            cur.r1 = off[t][p + 1];

            /*
                (p,N) : all len > t
                          == len >= t+1
            */
            cur.l2 = off[t + 1][p + 1];
            cur.r2 = off[t + 1][N];

            cur.len =
                (len[i] - zero[i]) +
                (cur.r1 - cur.l1) +
                (cur.r2 - cur.l2);

            if (!have || better(cur, best)) {
                have = true;
                best = cur;
            }
        }

        ++cnt[len[i]];
    }

    /*
        If no non-zero candidate exists, every possible
        concatenation consists entirely of zeros.
    */
    if (!have) {
        cout << 0 << '\n';
        return 0;
    }

    string ans;

    ans.reserve(best.len);

    ans += S[best.idx].substr(best.zero);

    ans.append(
        G[best.t],
        best.l1,
        best.r1 - best.l1
    );

    ans.append(
        G[best.t + 1],
        best.l2,
        best.r2 - best.l2
    );

    cout << ans << '\n';
}