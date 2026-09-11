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

MX = 20000

def solve():
    N = I()
    b = [[0, 0, 0] for i in range(N)]

    MM = 0
    for i in range(N):
        b[i][0], b[i][1], b[i][2] = MI()
        MM = max(MM, b[i][1] + b[i][0])

    dp = [0 for j in range(MX + 9)]

    b.sort(key=lambda x: x[0] + x[1])
    for i in range(N):
        w = b[i][0]
        s = b[i][1]
        v = b[i][2]
        for j in range(MX, -1, -1):
            if j >= w and j - w <= s:
                dp[j] = max(dp[j - w] + v, dp[j])
    ans = 0
    for i in range(MM + 1):
        ans = max(ans, dp[i])
    print(ans)
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
