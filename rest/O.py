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
    a = [LI() for i in range(N)]
    dp = [0 for i in range(1 << N)]
    dp[0] = 1
    
    for i in range((1 << N) - 1):
        for j in range(N):
            # print(i, i.bit_count())
            if a[i.bit_count()][j] == 1 and i >> j & 1 == 0:
                dp[i ^ (1 << j)] += dp[i]
                dp[i ^ (1 << j)] %= M
                
    
    print(dp[(1 << N) - 1])
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
