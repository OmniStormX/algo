import atexit
import sys

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


def _print(*args, sep=" ", end="\n"):
    out.append(sep.join(map(str, args)) + end)


print = _print


@atexit.register
def _():
    sys.stdout.write("".join(out))


def binaryPartition(n, m, g):

    color = [0] * (m + n)

    def dfs(u, c):
        color[u] = c

        for v in g[u]:

            if color[v] == 0:
                if not dfs(v, -c):
                    return False

            elif color[v] == color[u]:
                return False

        return True

    """
    二分图最大匹配数量
    其中正的是在图 A, 负的在图 B
    """
    ans = 0
    for i in range(n + m):
        if color[i] == 0:
            ans += dfs(i, 1)


n, m, e = MI()

g = [[] for i in range(n + m)]
for i in range(n):
    u, v = MI()
    u -= 1
    v = v - 1 + m
    g[u].append(v)
    g[v].append(u)
print()
