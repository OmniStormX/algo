import atexit
import sys

out = []


def I():
    return int(sys.stdin.readline().strip())


def MI():
    return map(int, sys.stdin.readline().strip().split())


def LI():
    return list(map(int, sys.stdin.readline().strip().split()))


def IS():
    return sys.stdin.readline().strip()


def _print(*args, sep=" ", end="\n"):
    out.append(sep.join(map(str, args)) + end)


print = _print


@atexit.register
def _():
    sys.stdout.write("".join(out))


def check(T, n, g):
    """
    只考虑权值 >= T 的边，
    判断这些边构成的图是否为二分图。
    """
    color = [-1] * n

    def dfs(u, c):
        color[u] = c

        for v, w in g[u]:
            if w < T:
                continue

            if color[v] == -1:
                if not dfs(v, c ^ 1):
                    return False

            elif color[v] == color[u]:
                return False

        return True

    for i in range(n):
        if color[i] == -1 and not dfs(i, 0):
            return False

    return True


N, M = MI()

g = [[] for _ in range(N)]
mx = 0

for _ in range(M):
    a, b, c = MI()
    a -= 1
    b -= 1

    g[a].append((b, c))
    g[b].append((a, c))

    mx = max(mx, c)


l = 0  # 不可行侧
r = mx + 1  # 一定可行，因为此时没有任何边

while l + 1 < r:
    mid = (l + r) // 2

    if check(mid, N, g):
        r = mid
    else:
        l = mid

print(l)
