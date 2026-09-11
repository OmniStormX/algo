import sys
from types import GeneratorType

sys.setrecursionlimit(10**7)


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

NN = 500009
f = fenwick(2 * NN)

def solve():
    
    N = I()
    s = SI()
    
    
    def g(x):
        return x + NN

    f.add(g(0), 1)
    ans = 0
    state = 0
    
    for i in range(N):
        if s[i] == 'A':
            state += 1
            ans += f.query(1, g(state - 1))
            f.add(g(state), 1)
        elif s[i] == 'B':
            state -= 1
            ans += f.query(1, g(state - 1))
            f.add(g(state), 1)
        else:
            ans += f.query(1, g(state - 1))
            f.add(g(state), 1)

    print(ans)

    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
