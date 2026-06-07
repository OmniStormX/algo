import copy
import sys
from types import GeneratorType
from math import gcd
from math import ceil
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
M = 998244353
Inf = 10**9 + 7


def I():
    return int(sys.stdin.readline().strip())


def MI():
    return map(int, sys.stdin.readline().strip().split())


def LI():
    return list(map(int, sys.stdin.readline().strip().split()))


def IS():
    return sys.stdin.readline().strip()


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
    if type(x) == str:
        out.append(x)
    else:
        out.append(str(x))


def solve():
    n, a, b = MI()
    _card = [0 for i in range(13)]

    for x in LI():
        _card[x - 1] += 1

    lim = 1
    ans = 0
    for j in range(11):
        lim *= 3

    for S in range(lim):
        flag = True
        score = 0
        card = copy.deepcopy(_card)
        for i in range(11):
            c = S % 3
            S = S // 3
            if card[i] < c or card[i + 1] < c or card[i + 2] < c:
                flag = False
            else:
                card[i] -= c
                card[i + 1] -= c
                card[i + 2] -= c
                score += b * c
        if not flag:
            continue

        if a >= b:
            for j in range(13):
                score += card[j] // 3 * a
        else:
            for j in range(11):
                k = min(card[j], card[j + 1], card[j + 2])
                score += k * b
                card[j] -= k
                card[j + 1] -= k
                card[j + 2] -= k
            for j in range(13):
                score += card[j] // 3 * a

        ans = max(ans, score)

    print(ans)


t = 1

for _ in range(t):
    solve()
print("\n".join(out))
