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


class LazySegmentTree:
    def __init__(self, n, init_val):
        self.n = n
        self.INF = init_val
        self.size = 1
        while self.size < n:
            self.size <<= 1
        self.tree = [self.INF] * (2 * self.size)
        self.lazy = [0] * (2 * self.size)

    def _apply(self, node, val):
        self.tree[node] += val
        self.lazy[node] += val

    def _push(self, node):
        if self.lazy[node] != 0:
            self._apply(node * 2, self.lazy[node])
            self._apply(node * 2 + 1, self.lazy[node])
            self.lazy[node] = 0

    def _range_add(self, l, r, val, node, nl, nr):
        # 半开区间 [l, r)
        if l <= nl and nr <= r:
            self._apply(node, val)
            return
        self._push(node)
        mid = (nl + nr) // 2
        if l < mid:
            self._range_add(l, r, val, node * 2, nl, mid)
        if r > mid:
            self._range_add(l, r, val, node * 2 + 1, mid, nr)
        self.tree[node] = max(self.tree[node * 2], self.tree[node * 2 + 1])

    def range_add(self, l, r, val):
        # 闭区间 [l, r] 统一转换为半开区间 [l, r+1)
        self._range_add(l, r + 1, val, 1, 0, self.size)

    def _set(self, pos, value, node, nl, nr):
        if nl + 1 == nr:  # 叶子节点
            self.tree[node] = value
            self.lazy[node] = 0
            return
        self._push(node)
        mid = (nl + nr) // 2
        if pos < mid:
            self._set(pos, value, node * 2, nl, mid)
        else:
            self._set(pos, value, node * 2 + 1, mid, nr)
        self.tree[node] = max(self.tree[node * 2], self.tree[node * 2 + 1])

    def set(self, pos, value):
        self._set(pos, value, 1, 0, self.size)

    def _query_max(self, l, r, node, nl, nr):
        if l <= nl and nr <= r:
            return self.tree[node]
        self._push(node)
        mid = (nl + nr) // 2
        res = self.INF
        if l < mid:
            res = max(res, self._query_max(l, r, node * 2, nl, mid))
        if r > mid:
            res = max(res, self._query_max(l, r, node * 2 + 1, mid, nr))
        return res

    def query_max(self, l, r):
        # 闭区间 [l, r]
        return self._query_max(l, r + 1, 1, 0, self.size)


def solve():
    n, m = MI()
    R = [[] for i in range(n + 1)]
    for i in range(m):
        x, y, z = MI()
        R[y].append((x, z))
    # print(R)
    seg = LazySegmentTree(n + 1, -Inf)
    seg.set(0, 0)
    for i in range(1, n + 1):
        v = seg.query_max(0, i - 1)
        seg.set(i, v)
        for l, v in R[i]:
            seg.range_add(l, i, v)
    print(seg.query_max(0, n))
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
