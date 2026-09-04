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
        self.min = [10**9 for _ in range(n)]
        self.max = [-(10**9) for _ in range(n)]

    def find(self, x):
        if self.t[x] == x:
            return self.t[x], self.val[x]

        fa, val = self.find(self.t[x])
        self.t[x] = fa
        self.val[x] += val
        self.max[fa] = fmax(self.max[fa], self.val[x])
        self.min[fa] = fmin(self.min[fa], self.val[x])
        return self.t[x], self.val[x]

    # x eat y
    def merge(self, x, y):
        fax, valx = self.find(x)
        fay, valy = self.find(y)
        if fax == fay:
            if (((valx - valy) % NN + NN) % NN) != 1:
                return False
            return True
        minx, maxx = self.min[fax], self.max[fax]
        # miny, maxy = self.min[fay], self.max[fay]
        delta = valy - valx + 1
        self.val[fax] = (self.val[fax] + delta) % NN
        if self.val[fax] < 0:
            self.val[fax] += NN
        self.t[fax] = fay
        self.max[fay] = fmax(self.max[fay], maxx + delta)
        self.min[fay] = fmin(self.min[fay], minx + delta)
        return True

    def mergeSame(self, x, y):
        fax, valx = self.find(x)
        fay, valy = self.find(y)
        if fax == fay:
            if (valx % NN + NN) % NN != (valy % NN + NN) % NN:
                return False
            return True

        delta = valy - valx
        self.val[fax] = (self.val[fax] + delta) % NN
        if self.val[fax] < 0:
            self.val[fax] += NN

        self.t[fax] = fay
        return True


def solve():
    N, K = MI()

    ans = 0
    dsu = Dsu_with_weight(N + 9)
    for i in range(K):
        op, X, Y = MI()
        if X > N or Y > N:
            continue
        if op == 1:
            if dsu.mergeSame(X, Y):
                ans += 1
        else:
            if X == Y:
                continue
            if dsu.merge(X, Y):
                ans += 1
    print(K - ans)


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
