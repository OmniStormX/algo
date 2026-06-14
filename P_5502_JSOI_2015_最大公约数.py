import sys
from math import gcd
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

    def DAC(l, r):
        nonlocal a
        if l == r:
            return a[l]

        m = (l + r) // 2
        g = a[m]
        ans = a[m]
        ll = rr = m
        while l <= ll and rr <= r:
            while rr < r and gcd(a[rr + 1], g) % g == 0:
                rr += 1
            ans = max(ans, (rr - ll + 1) * g)
            if ll - 1 < l:
                break
            ll -= 1
            g = gcd(g, a[ll])
            ans = max(ans, (rr - ll + 1) * g)

        ll = rr = m
        g = a[m]
        while l <= ll and rr <= r:
            while ll > l and gcd(a[ll - 1], g) % g == 0:
                ll -= 1
            ans = max(ans, (rr - ll + 1) * g)
            if rr + 1 > r:
                break
            rr += 1
            g = gcd(g, a[rr])
            ans = max(ans, (rr - ll + 1) * g)
        return max(ans, DAC(l, m), DAC(m + 1, r))

    print(DAC(0, N - 1))
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
