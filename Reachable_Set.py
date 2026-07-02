import heapq
import sys
from types import GeneratorType
from collections import defaultdict
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
    out.append(x)


def solve():
    N, M = MI()
    pq = [0]
    cnt = 0
    e = [[] for i in range(N)]
    for i in range(M):
        u, v = MI()
        u -= 1
        v -= 1
        e[u].append(v)

    S = set()
    M = set()
    for i in range(N):
        for v in e[i]:
            if v not in M:
                M.add(v)
                heapq.heappush(pq, v)
        while pq and pq[0] <= i:
            S.add(pq[0])
            heapq.heappop(pq)
        if len(S) < i + 1:
            o(-1)
        else:
            o(len(pq))
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(map(str, out)))
