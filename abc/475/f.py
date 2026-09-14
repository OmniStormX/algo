import atexit
import heapq
import sys
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


H, W = MI()
S = [IS() for _ in range(H)]

# 让 H <= W，平方较小的维度
if H > W:
    S = ["".join(x) for x in zip(*S)]
    H, W = W, H

ans = 1

pre = [0] * (W + 1)
for bottom in range(H):
    f = False
    active = [0] * W
    for i in range(W):
        if S[bottom][i] == ".":
            f = True
    if not f:
        continue
    for top in range(bottom, H):
        f = False
        for i in range(W):
            if S[top][i] == ".":
                f = True
                active[i] = f
        if not f:
            continue

        last_bottom = -1
        last_top = -1
        for i in range(W):
            if S[bottom][i] == ".":
                last_bottom = i
            if S[top][i] == ".":
                last_top = i
            pre[i] = pre[i - 1] + active[i]
            if active[i]:
                ans += pre[min(last_top, last_bottom)]

print(ans)
