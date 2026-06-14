import sys
from types import GeneratorType

sys.setrecursionlimit(10**7)

Comb = False


def bootstrap(f, stk=[]):
    def wrappedfunc(*args, **kwargs):
        if stk:
            return f(*args, **kwargs)
        else:
            to = f(*args, **kwargs)
            while True:
                if type(to) is GeneratorType:
                    stk.append(to)
                    to = next(to)
                else:
                    stk.pop()
                    if not stk:
                        break
                    to = stk[-1].send(to)
            return to

    return wrappedfunc


out = []


def I():
    return int(sys.stdin.readline().strip())


def MI():
    return map(int, sys.stdin.readline().strip().split())


def LI():
    return list(map(int, sys.stdin.readline().strip().split()))


def LFI():
    return list(map(float, sys.stdin.readline().strip().split()))


def IS():
    return sys.stdin.readline().strip()


M = 10**9 + 7
Inf = 10**18 + 7


def power(x, y):
    ans = 1
    while y > 0:
        if y & 1 == 1:
            ans = ans * x % M
        y >>= 1
        x = x * x % M
    return ans


def o(x):
    global out
    if type(x) == str:  # noqa: E721
        out.append(x)
    else:
        out.append(str(x))


class Comb:
    def __init__(self, max_n, mod):
        self.mod = mod
        self.max_n = max_n

        self.fac = [1] * (max_n + 1)
        self.ifac = [1] * (max_n + 1)

        for i in range(1, max_n + 1):
            self.fac[i] = self.fac[i - 1] * i % mod

        self.ifac[max_n] = pow(self.fac[max_n], mod - 2, mod)

        for i in range(max_n, 0, -1):
            self.ifac[i - 1] = self.ifac[i] * i % mod

    def factor(self, n):
        """n!"""
        return self.fac[n]

    def inv_factor(self, n):
        """(n!)^{-1}"""
        return self.ifac[n]

    def C(self, n, k):
        """组合数 C(n, k)"""
        if k < 0 or k > n:
            return 0
        return self.fac[n] * self.ifac[k] % self.mod * self.ifac[n - k] % self.mod

    def P(self, n, k):
        """排列数 P(n, k)"""
        if k < 0 or k > n:
            return 0
        return self.fac[n] * self.ifac[n - k] % self.mod


def exgcd(a, b):
    if b == 0:
        return a, 1, 0

    d, x1, y1 = exgcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return d, x, y


def inv(a, mod):
    print(a, mod)
    d, x, y = exgcd(a, mod)
    print(d, x, y)
    if d != 1:
        return None

    return x % mod


def solve():
    n, m = MI()
    e = [[] for i in range(n)]
    f = [0 for i in range(n)]

    for v in range(n - 1):
        u, v = MI()
        u -= 1
        v -= 1
        e[u].append(v)
        e[v].append(u)

    def dfs(u, fa):
        f[u] = 1
        for v in e[u]:
            if v == fa:
                continue
            dfs(v, u)
            f[u] = f[u] * (f[v] + 1) % m

    up = [1] * n
    ans = [0] * n

    def dfs2(u, fa):
        ans[u] = f[u] * up[u] % m

        ch = []
        for v in e[u]:
            if v == fa:
                continue
            ch.append(v)

        k = len(ch)

        pre = [1] * (k + 1)
        suf = [1] * (k + 1)

        for i in range(k):
            pre[i + 1] = pre[i] * (f[ch[i]] + 1) % m

        for i in range(k - 1, -1, -1):
            suf[i] = suf[i + 1] * (f[ch[i]] + 1) % m

        for i, v in enumerate(ch):
            up[v] = (up[u] * pre[i] % m * suf[i + 1] % m + 1) % m
            dfs2(v, u)

    dfs(0, -1)
    dfs2(0, -1)

    for v in range(n):
        o(ans[v])
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
