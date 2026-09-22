#include <bits/stdc++.h>
using namespace std;
// #define print(x)    cerr << #x << " = " << x << std::endl
// #define print(x)    42
void solve() {
    int n, q;
    cin >> n >> q;
    string s;
    cin >> s;
    // cnt = -3
    int L = sqrt(n);
    int N = (n - 1) / L;
    // [0, N]
    vector<int> cntl(N + 1, 0), cntr(N + 1, 0), sum(N + 1, 0);
    vector<int> tag(N + 1, 0);
    auto f = [&](int x) {
        return x / L;
    };
    auto rebuild = [&](int p) {
        int l = p * L, r = min(p * L + L - 1, n - 1);
        if (tag[p]) {
            for (int i = l; i <= r; i++) {
                if (s[i] == '(') s[i] = ')';
                else {
                    s[i] = '(';
                }
            }
			tag[p] ^= 1;
        }
        int cnt = 0;
        cntl[p] = cntr[p] = sum[p] = 0;
        for (int i = l; i <= r; i++) {
            if (s[i] == ')') {
                cnt++;
            } else {
                cnt--;
            }
            cntl[p] = max(cntl[p], cnt);
			cntr[p] = max(cntr[p], -cnt);
        }
        cnt = 0;
        sum[p] = 0;
        for (int i = l; i <= r; i++) {
            sum[p] += s[i] == '(' ? 1 : -1;
        }
    };
    auto addTag = [&] (int p) {
        tag[p] ^= 1;
        swap(cntl[p], cntr[p]);
        sum[p] *= -1;
    };
    for (int i = 0; i <= N; i++) {
        rebuild(i);
    }
	// q * (n / L + L)
    int l, r;
    for (int i = 0; i < q; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            cin >> l >> r;
            l--;
            r--;
            int bl = f(l), br = f(r);
            if (bl == br) {
                for (int j = l; j <= r; j++) {
                    s[j] = (s[j] == '(' ? ')' : '(');
                }
                rebuild(bl);
            } else {
                for (int j = l; j < bl * L + L; j++) {
                    s[j] = (s[j] == '(' ? ')' : '(');
                }
                rebuild(bl);
                for (int j = br * L; j <= r; j++) {
                    s[j] = (s[j] == '(' ? ')' : '(');
                }
                rebuild(br);
                for (int j = bl + 1; j < br; j++) addTag(j);
            }
        } else {
            int l, r;
            cin >> l >> r;
            l--;
            r--;
            bool ok = 1;
            int bl = f(l), br = f(r);
            if (bl == br) {
                rebuild(bl);
                int cnt = 0;
                for (int j = l; j <= r; j++) {
                    if (s[j] == ')') cnt--;
                    else {
                        cnt++;
                    }
                    if (cnt < 0) {
                        ok = 0;
                        break;
                    }
                }
                if (cnt != 0) ok = 0;
            } else {
                int cnt = 0;
                rebuild(bl);
                for (int j = l; j < bl * L + L; j++) {
                    if (s[j] == '(') {
                        cnt++;
                    } else {
                        cnt--;
                    }
                    if (cnt < 0) {
                        ok = 0;
                        break;
                    }
                }
                for (int j = bl + 1; j < br; j++) {
                    if (cnt < cntl[j]) {
                        ok = 0;
                        break;
                    }
                    cnt += sum[j];
                }
                rebuild(br);
                for (int j = br * L; j <= r; j++) {
                    if (s[j] == '(') {
                        cnt++;
                    } else {
                        cnt--;
                    }
                    if (cnt < 0) {
                        ok = 0;
                        break;
                    }
                }
				if (cnt != 0) ok = 0;
            }
            cout << (ok ? "YES" : "NO") << "\n";
        }
    }
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    solve();
    return 0;
}