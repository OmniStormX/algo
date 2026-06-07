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


MAX = 200001
fac = [0 for i in range(MAX)]
ifac = [0 for i in range(MAX)]
fac[0] = ifac[0] = 1
for i in range(1, MAX):
    fac[i] = fac[i - 1] * i % M
    ifac[i] = power(fac[i], M - 2)


def C(n, m):
    return fac[n] * ifac[m] * ifac[n - m] % M


def solve():
    h, w, n = MI()
    d = [(0, 0) for i in range(n)]
    for i in range(n):
        u, v = MI()
        d[i] = (u, v)
    d.append((h, w))
    d.sort()
    dp = [0 for i in range(n + 1)]
    dp[0] = C(d[0][0] + d[0][1] - 2, d[0][0] - 1)

    for i in range(1, n + 1):
        h1, v1 = d[i]
        dp[i] = C(d[i][0] + d[i][1] - 2, d[i][0] - 1)
        for j in range(i):
            h0, v0 = d[j]
            if h1 >= h0 and v1 >= v0:
                dp[i] -= C(h1 - h0 + v1 - v0, h1 - h0) * dp[j] % M
                if dp[i] < 0:
                    dp[i] += M
    # print(dp)
    print(dp[n])
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
