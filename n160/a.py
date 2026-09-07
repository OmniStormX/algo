import atexit
import bisect
import sys

out = []
sys.setrecursionlimit(1000000)


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


# def _print(*args, sep=" ", end="\n"):
#     out.append(sep.join(map(str, args)) + end)


# print = _print


@atexit.register
def _():
    sys.stdout.write("".join(out))


class Fenwick_not_diff:

    __slots__ = ("a", "n", "t")

    def __init__(self, n):
        self.t = [0] * (n + 1)
        self.a = [0] * (n + 1)
        self.n = n

    def modify(self, x, y):
        self.a[x] = max(self.a[x], y)

        while x <= self.n:
            self.t[x] = max(self.t[x], y)
            x += x & -x

    def query(self, l, r):
        ans = 0
        while r >= l:
            if r - (r & -r) + 1 >= l:
                ans = max(ans, self.t[r])
                r -= r & -r
            else:
                ans = max(ans, self.a[r])
                r -= 1
        return ans


class Fenwick:
    __slot__ = ("t", "n")

    def __init__(self, n):
        self.t = [0 for _ in range(n + 1)]
        self.n = n

    def add(self, x, y):
        while x <= self.n:
            self.t[x] += y
            x += x & -x

    def sum(self, x):
        ans = 0
        while x > 0:
            ans += self.t[x]
            x -= x & -x
        return ans

    def sum_range(self, l, r):
        """[l, r]"""
        return self.sum(r) - self.sum(l - 1)


n, t = MI()
Q = []
z = [-(10**10)]
for i in range(n):
    l, r = MI()
    z.append(l)
    z.append(r)
    z.append(r - t)
    Q.append((l, r))
z = sorted(set(z))


def Z(x):
    return bisect.bisect_left(z, x)


ans = 0

N = len(z)

f = Fenwick(N)

Q.sort(key=lambda x: (x[1], -x[0]))

for l, r in Q:

    if r - l == t:
        ans += f.sum_range(Z(l), Z(r))
    f.add(Z(l), 1)

p = [[] for _ in range(N)]

for l, r in Q:
    p[Z(l)].append(r)

for i in range(N):
    if p[i]:
        p[i].sort()
for l, r in Q:
    if l > r - t:
        v = Z(r - t)
        ans += bisect.bisect_left(p[v], r) - bisect.bisect_left(p[v], l)

print(ans)
