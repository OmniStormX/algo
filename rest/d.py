import math
import sys
from types import GeneratorType

sys.setrecursionlimit(10**7)
Factor = False

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


def SI():
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


if Factor:
    MAX = 200001
    fac = [0 for i in range(MAX)]
    ifac = [0 for i in range(MAX)]
    fac[0] = ifac[0] = 1
    for i in range(1, MAX):
        fac[i] = fac[i - 1] * i % M
        ifac[i] = power(fac[i], M - 2)

    def C(n, m):
        return fac[n] * ifac[m] * ifac[n - m] % M

MX = 20000

class fenwick():

    def __init__(self, n):
        self.t = [0 for i in range(n + 1)]
        self.N = n
        pass

    def add(self, x, v):
        while x <= self.N:
            self.t[x] += v
            x += x & -x
    
    def query(self, l, r):
        ans = 0
        return self._query(r) - self._query(l - 1)
    
    def _query(self, x):
        assert x >= 0
        ans = 0
        while x > 0:
            ans += self.t[x]
            x -= x & -x
        return ans

    pass


def solve():
    C, D = MI()

    mx = 1
    d = 0
    while mx <= C + D:
        mx *= 10
        d += 1
    k = 1
    sqrt = math.isqrt
    ans = 0 
    for _ in range(d + 1):
        k *= 10
        l, r = k // 10, k - 1
        l = max(l, C)
        r = min(r, C + D)
        if l <= r:
            ll = int(sqrt(l + C * k))
            if ll * ll < l + C * k:
                ll += 1
            rr = int(sqrt(r + C * k))
            if ll <= rr:
                ans += rr - ll + 1
    print(ans)


    pass


t = I()

for _ in range(t):
    solve()
print("\n".join(out))
