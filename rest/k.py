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
    n, k = MI()
    a = LI()

    dp = [[-1, -1] for i in range(k + 1)]

    dp[0][0] = 0
    dp[0][1] = 0

    for i in range(1, k + 1):
        f = False
        f0 = 0
        f1 = 0
        for x in a:
            if i >= x:
                f = True
                if dp[i - x][0] == 0:
                    f0 = 1
                if dp[i - x][1] == 0:
                    f1 = 1
        dp[i][1] = f0
        dp[i][0] = f1

        if not f:
            dp[i][0] = 0
            dp[i][1] = 0
    if dp[k][0]:
        print('First')
    else:
        print('Second')
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
