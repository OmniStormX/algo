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
    K = IS()
    D = I()
    L = len(K)
    dp = [[[-1, -1] for i in range(D)] for j in range(L)]

    @bootstrap
    def calc(t, isNum: bool, isLimit: bool, sum):
        nonlocal K
        nonlocal D
        if t == L:
            if isNum and sum == 0:
                yield 1
            else:
                yield 0
        if not isLimit and dp[t][sum][isNum] != -1:
            yield dp[t][sum][isNum]
        ans = 0
        if not isNum:
            ans += yield calc(t + 1, isNum, False, sum)
            ans %= M
        
        for i in range(0 if isNum else 1, int(ord(K[t]) - ord('0') + 1) if isLimit else 10):
            ans += yield calc(t + 1, isNum | bool(i > 0), isLimit and bool(i == ord(K[t]) - ord('0')), (sum + i) % D)
            ans %= M
        if not isLimit:
            dp[t][sum][isNum] = ans
        yield ans
    print(calc(0, False, True, 0))
    # print(dp)
    pass


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
