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


class Line:
    __slots__ = ("k", "b")

    def __init__(self, k, b):
        self.k = k
        self.b = b

    def f(self, x):
        return self.k * x + self.b


def bad(a, b, c):
    return (b.b - a.b) * (a.k - c.k) >= (c.b - a.b) * (a.k - b.k)


def fmax(x, y):
    if x > y:
        return x
    return y


def fmin(x, y):
    if x < y:
        return x
    return y


NN = 3


class Dsu_with_weight:

    def __init__(self, n):
        self.t = [i for i in range(n)]
        self.val = [0 for _ in range(n)]
        self.end = [i for i in range(n)]

    def find(self, x):
        if self.t[x] == x:
            return self.t[x], self.val[x]
        fa, val = self.find(self.t[x])
        self.t[x] = fa
        self.val[x] += val
        return self.t[x], self.val[x]

    # x -> y
    def merge(self, x, y):
        fax, valx = self.find(x)
        fay, valy = self.find(y)
        if fax == fay:
            return True
        self.end[fay] = self.end[fax]
        self.val[fax] += valy + 1
        self.t[fax] = fay
        return True


def solve():
    K = I()

    dsu = Dsu_with_weight(30009)
    for i in range(K):
        op, *args = sys.stdin.readline().strip().split()
        if op == "M":
            x, y = args
            x = int(x)
            y = int(y)
            fy, vy = dsu.find(y)
            endy = dsu.end[fy]
            dsu.merge(x, endy)

        else:
            s = args[0]
            fx, vx = dsu.find(int(s))
            o(vx)


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
