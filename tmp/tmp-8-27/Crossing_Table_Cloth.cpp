#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll MOD = 998244353;
const int C = 26;

struct Node {
    int nxt[C];
    int fail;
    bool bad;

    Node() {
        fill(nxt, nxt + C, -1);
        fail = 0;
        bad = false;
    }
};

using Matrix = vector<vector<ll>>;

Matrix multiply(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C(n, vector<ll>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < n; j++) {
                C[i][j] += A[i][k] * B[k][j] % MOD;
                if (C[i][j] >= MOD) C[i][j] -= MOD;
            }
        }
    }

    return C;
}

Matrix matrix_power(Matrix A, long long e) {
    int n = A.size();
    Matrix res(n, vector<ll>(n, 0));

    for (int i = 0; i < n; i++) {
        res[i][i] = 1;
    }

    while (e > 0) {
        if (e & 1) res = multiply(res, A);
        A = multiply(A, A);
        e >>= 1;
    }

    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N;
    int K;
    cin >> N >> K;

    vector<Node> trie(1);

    for (int i = 0; i < K; i++) {
        string s;
        cin >> s;

        int v = 0;
        for (char ch : s) {
            int c = ch - 'a';
            if (trie[v].nxt[c] == -1) {
                trie[v].nxt[c] = trie.size();
                trie.emplace_back();
            }
            v = trie[v].nxt[c];
        }

        trie[v].bad = true;
    }

    queue<int> q;

    for (int c = 0; c < C; c++) {
        int u = trie[0].nxt[c];

        if (u == -1) {
            trie[0].nxt[c] = 0;
        } else {
            trie[u].fail = 0;
            q.push(u);
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        trie[v].bad = trie[v].bad || trie[trie[v].fail].bad;

        for (int c = 0; c < C; c++) {
            int u = trie[v].nxt[c];

            if (u == -1) {
                trie[v].nxt[c] = trie[trie[v].fail].nxt[c];
            } else {
                trie[u].fail = trie[trie[v].fail].nxt[c];
                q.push(u);
            }
        }
    }

    int sz = trie.size();

    Matrix M(sz, vector<ll>(sz, 0));

    for (int v = 0; v < sz; v++) {
        if (trie[v].bad) continue;

        for (int c = 0; c < C; c++) {
            int u = trie[v].nxt[c];
            if (trie[u].bad) continue;
            M[v][u]++;
            M[v][u] %= MOD;
        }
    }

    Matrix P = matrix_power(M, N);

    ll ans = 0;

    for (int v = 0; v < sz; v++) {
        if (!trie[v].bad) {
            ans += P[0][v];
            ans %= MOD;
        }
    }

    cout << ans << '\n';

    return 0;
}