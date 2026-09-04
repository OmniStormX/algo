import sys

input = sys.stdin.readline

N, C = map(int, input().split())
h = list(map(int, input().split()))

dp = [0] * N


class Line:
    __slots__ = ("k", "b")

    def __init__(self, k, b):
        self.k = k
        self.b = b

    def value(self, x):
        return self.k * x + self.b


def bad(a, b, c):
    return (b.b - a.b) * (a.k - c.k) >= (c.b - a.b) * (a.k - b.k)


def solve(l, r):
    if l == r:
        return

    mid = (l + r) >> 1

    solve(l, mid)

    lines = []

    for i in range(l, mid + 1):
        lines.append(Line(-2 * h[i], dp[i] + h[i] * h[i]))

    lines.sort(key=lambda x: x.k)

    hull = []

    for line in lines:
        while len(hull) >= 2 and bad(hull[-2], hull[-1], line):
            hull.pop()
        hull.append(line)

    def query(x):
        lo = 0
        hi = len(hull) - 1

        while lo < hi:
            m = (lo + hi) >> 1

            if hull[m].value(x) <= hull[m + 1].value(x):
                hi = m
            else:
                lo = m + 1

        return hull[lo].value(x)

    for i in range(mid + 1, r + 1):
        best = query(h[i])

        dp[i] = min(dp[i] if dp[i] else 10**30, h[i] * h[i] + C + best)

    solve(mid + 1, r)


INF = 10**30

for i in range(1, N):
    dp[i] = INF

solve(0, N - 1)

print(dp[-1])
