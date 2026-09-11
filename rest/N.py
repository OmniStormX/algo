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


def solve():
    N = I()
    a = LI()

    dp = [[[Inf, Inf] for j in range(N)] for i in range(N)]
    
    for i in range(N):
        dp[i][i][0] = a[i]
        dp[i][i][1] = 0
    

    for l in range(2, N + 1):
        for i in range(N):
            if i + l - 1 >= N:
                break
            for j in range(i, i + l - 1):
                if dp[i][i + l - 1][1] > dp[i][j][1] + dp[j + 1][i + l - 1][1] + dp[i][j][0] + dp[j + 1][i + l - 1][0]:
                    dp[i][i + l - 1][0] = dp[i][j][0] + dp[j + 1][i + l - 1][0]
                    dp[i][i + l - 1][1] = dp[i][j][1] + dp[j + 1][i + l - 1][1] + dp[i][j][0] + dp[j + 1][i + l - 1][0]

    print(dp[0][N - 1][1])
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
