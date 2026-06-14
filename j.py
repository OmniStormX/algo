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
Inf = 10**9 + 7


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


def solve():
    N = I()
    a = LI()

    dp = [[[-1 for k in range(N + 1)] for j in range(N + 1)] for i in range(N + 1)]

    def dfs(c1, c2, c3):
        ans = 0
        if dp[c1][c2][c3] != -1:
            return dp[c1][c2][c3]
        if c1 == 0 and c2 == 0 and c3 == 0:
            return 0
        if c1 > 0:
            ans += c1 / N * dfs(c1 - 1, c2, c3)
        if c2 > 0:
            ans += c2 / N * dfs(c1 + 1, c2 - 1, c3)
        if c3 > 0:
            ans += c3 / N * dfs(c1, c2 + 1, c3 - 1)
        ans += 1
        if c1 + c2 + c3 > 0:
            ans = ans / (c1 + c2 + c3) * N
        dp[c1][c2][c3] = ans
        return ans

    c = [0, 0, 0]
    for x in a:
        c[x - 1] += 1

    print(dfs(c[0], c[1], c[2]))

    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
