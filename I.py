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


M = 998244353
Inf = 10**9 + 7


def qpow(x, y):
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


def solve():
    n = I()
    p = LFI()

    dp = [[0 for j in range(n + 1)] for i in range(n + 1)]
    dp[0][0] = 1

    for i in range(1, n + 1):
        for j in range(0, i + 1):
            dp[i][j] = dp[i - 1][j] * (1 - p[i - 1])
            if j > 0:
                dp[i][j] += dp[i - 1][j - 1] * p[i - 1]

    ans = 0
    for i in range(0, n + 1):
        if i > n - i:
            ans += dp[n][i]
    print(ans)


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
