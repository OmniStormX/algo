import sys
from collections import deque
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
    if type(x) == str:
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
    __slots__ = ("b", "k")

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


INF = 10**18


class Edge:
    __slots__ = ("cap", "rev", "to")

    def __init__(self, to, cap, rev):
        self.to = to
        self.cap = cap
        self.rev = rev


class Dinic:
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]

    def add_edge(self, u, v, c):
        self.g[u].append(Edge(v, c, len(self.g[v])))
        self.g[v].append(Edge(u, 0, len(self.g[u]) - 1))

    def bfs(self, s, t):
        self.level = [-1] * self.n
        q = deque([s])
        self.level[s] = 0

        while q:
            v = q.popleft()
            for e in self.g[v]:
                if e.cap > 0 and self.level[e.to] == -1:
                    self.level[e.to] = self.level[v] + 1
                    q.append(e.to)

        return self.level[t] != -1

    def dfs(self, v, t, f):
        if v == t:
            return f

        while self.it[v] < len(self.g[v]):
            e = self.g[v][self.it[v]]

            if e.cap > 0 and self.level[e.to] == self.level[v] + 1:
                d = self.dfs(e.to, t, min(f, e.cap))
                if d:
                    e.cap -= d
                    self.g[e.to][e.rev].cap += d
                    return d

            self.it[v] += 1

        return 0

    def max_flow(self, s, t):
        flow = 0

        while self.bfs(s, t):
            self.it = [0] * self.n

            while True:
                f = self.dfs(s, t, INF)
                if f == 0:
                    break
                flow += f

        return flow


def solve():
    s0 = []
    s1 = []
    N = I()
    a = LI()
    a.sort()
    for x in a:
        if x <= 0:
            s0.append(x)
        else:
            s1.append(x)
    s1.reverse()
    cur = 0
    ans = 0
    while s0 and s1:
        if abs(s0[-1] - cur) <= abs(s1[-1] - cur):
            ans += abs(cur - s0[-1])
            cur = s0[-1]
            s0.pop()
        else:
            ans += abs(cur - s1[-1])
            cur = s1[-1]
            s1.pop()

    while s0:
        ans += abs(cur - s0[-1])
        cur = s0[-1]
        s0.pop()
    while s1:
        ans += abs(cur - s1[-1])
        cur = s1[-1]
        s1.pop()
    print(ans)


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
