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


from collections import deque


class Line:
    __slots__ = ("k", "b")

    def __init__(self, k, b):
        self.k = k
        self.b = b

    def f(self, x):
        return self.k * x + self.b


def bad(a, b, c):
    return (b.b - a.b) * (a.k - c.k) >= (c.b - a.b) * (a.k - b.k)


def solve():
    N, C = MI()
    h = LI()

    dp = [0] * N

    q = deque()

    q.append(Line(-2 * h[0], h[0] * h[0]))

    for i in range(1, N):
        x = h[i]

        while len(q) >= 2 and q[1].f(x) <= q[0].f(x):
            q.popleft()

        best = q[0].f(x)

        dp[i] = x * x + C + best

        newline = Line(-2 * x, dp[i] + x * x)

        while len(q) >= 2 and bad(
            q[-2],
            q[-1],
            newline,
        ):
            q.pop()

        q.append(newline)

    print(dp[-1])


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
