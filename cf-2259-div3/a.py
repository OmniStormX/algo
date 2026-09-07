import atexit
import sys

out = []
stdout = 1


def I():
    return int(sys.stdin.readline().strip())


def MI():
    return map(int, sys.stdin.readline().strip().split())


def LI():
    return list(map(int, sys.stdin.readline().strip().split()))


def LFI():
    return list(map(float, sys.stdin.readline().strip().split()))


def IS():
    """输入字符串"""
    return sys.stdin.readline().strip()


if stdout:

    def _print(*args, sep=" ", end="\n"):
        out.append(sep.join(map(str, args)) + end)

    print = _print


@atexit.register
def _():
    sys.stdout.write("".join(out))


class Dsu:

    def __init__(self, n):
        self.t = [i for i in range(n)]

    def find(self, x):
        if x == self.t[x]:
            return x
        self.t[x] = self.find(self.t[x])
        return self.t[x]

    def merge(self, x, y):
        fax, fay = self.find(x), self.find(y)
        if fax == fay:
            return
        self.t[fax] = fay


class Seg:
    def __init__(s, n, v=0):
        s.n = n
        s.sum = [0] * (4 * n)
        s.tag = [None] * (4 * n)
        s.sum[1] = n * v
        s.tag[1] = v

    def set(s, L, R, v):
        L = max(L, 1)
        R = min(R, s.n)
        if L > R:
            return

        def f(p, l, r):
            if L <= l and r <= R:
                s.sum[p] = (r - l + 1) * v
                s.tag[p] = v
                return

            m = (l + r) // 2

            if s.tag[p] is not None:
                x = s.tag[p]
                s.sum[p * 2] = (m - l + 1) * x
                s.sum[p * 2 + 1] = (r - m) * x
                s.tag[p * 2] = s.tag[p * 2 + 1] = x
                s.tag[p] = None

            if L <= m:
                f(p * 2, l, m)
            if R > m:
                f(p * 2 + 1, m + 1, r)

            s.sum[p] = s.sum[p * 2] + s.sum[p * 2 + 1]

        f(1, 1, s.n)

    def get(s, L, R):
        L = max(L, 1)
        R = min(R, s.n)
        if L > R:
            return 0

        def f(p, l, r):
            if L <= l and r <= R:
                return s.sum[p]

            m = (l + r) // 2

            if s.tag[p] is not None:
                x = s.tag[p]
                s.sum[p * 2] = (m - l + 1) * x
                s.sum[p * 2 + 1] = (r - m) * x
                s.tag[p * 2] = s.tag[p * 2 + 1] = x
                s.tag[p] = None

            ans = 0
            if L <= m:
                ans += f(p * 2, l, m)
            if R > m:
                ans += f(p * 2 + 1, m + 1, r)
            return ans

        return f(1, 1, s.n)


for _ in range(I()):
    n = I()
    b = LI()

    dif = [0] * (n + 1)

    for i, d in enumerate(b):
        if d > 0:
            l = max(0, i - d + 1)
            r = min(n - 1, i + d - 1)
            dif[l] += 1
            dif[r + 1] -= 1

    treasure_map_fin = ["1"] * n

    cur = 0
    for i in range(n):
        cur += dif[i]
        if cur:
            treasure_map_fin[i] = "0"

    ok = True

    for i, d in enumerate(b):
        if d == 0:
            if treasure_map_fin[i] != "1":
                ok = False
                break

        elif d > 0:
            l = i - d
            r = i + d

            if not (
                (l >= 0 and treasure_map_fin[l] == "1")
                or (r < n and treasure_map_fin[r] == "1")
            ):
                ok = False
                break

    if not ok or "1" not in treasure_map_fin:
        print(-1)
    else:
        print("".join(treasure_map_fin))
