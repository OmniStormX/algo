import atexit
import bisect
import heapq
import sys
from copy import copy
from types import GeneratorType

hpush = heapq.heappush
hpop = heapq.heappop

Comb = False
output = True


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


if output:

    def _print(*args, sep=" ", end="\n"):
        out.append(sep.join(map(str, args)) + end)

    print = _print

    @atexit.register
    def _():
        sys.stdout.write("".join(out))


class fenwick:

    def __init__(self, n):
        """x in [1, n]"""
        self.t = [0 for i in range(n + 1)]
        self.N = n

    def add(self, x, v):
        assert x <= self.N and x > 0

        while x <= self.N:
            self.t[x] += v
            x += x & -x

    def query(self, l, r):
        return self._query(r) - self._query(l - 1)

    def _query(self, x):
        assert x >= 0
        ans = 0
        while x > 0:
            ans += self.t[x]
            x -= x & -x
        return ans


N, M, K = MI()
T = IS()
S = [IS() for _ in range(N)]

hq = []

for i in range(N):
    S[i] = [0 if c == T[i] else 1 for i, c in enumerate(S[i])]


def Binary(s):
    ans = 0
    for c in s:
        ans = ans * 2 + c
    return ans


Z = []

for i in range(N):
    S[i] = Binary(S[i])
    Z.append(S[i])
Q = I()

dlt = {}

QQ = []
SS = copy(S)
for _ in range(Q):
    i, j = MI()
    i -= 1
    j -= 1
    QQ.append((i, j))
    SS[i] = SS[i] ^ (1 << (K - 1 - j))
    Z.append(SS[i])

Z = sorted(set(Z))


def z(x):
    return bisect.bisect_left(Z, x) + 1


n = len(Z)
f = fenwick(n + 1)
for x in S:
    f.add(z(x), 1)

for i, j in QQ:
    # print(f"sum = {f.query(1, n)}")
    f.add(z(S[i]), -1)
    # print(f"sum = {f.query(1, n)}")
    S[i] ^= 1 << (K - 1 - j)
    f.add(z(S[i]), 1)
    s = f.query(1, z(S[i]))
    # print(f"s = {s}")
    if s <= M and S[i] != (1 << K) - 1:
        print("Yes")
    else:
        print("No")
