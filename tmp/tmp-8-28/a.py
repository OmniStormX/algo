import math
from collections import deque

INF = 10**18
"""网络流中的无穷大值。"""


class Edge:
    __slots__ = ("cap", "rev", "to")

    def __init__(self, to, cap, rev):
        self.to = to
        self.cap = cap
        self.rev = rev


class Dinic:
    def __init__(self, n):
        self.n = n
        self.g = [[] for _ in range(n)]

    def add_edge(self, u, v, c):
        self.g[u].append(Edge(v, c, len(self.g[v])))
        self.g[v].append(Edge(u, 0, len(self.g[u]) - 1))

    def bfs(self, s, t):
        self.level = [-1] * self.n
        q = deque([s])
        self.level[s] = 0

        while q:
            v = q.popleft()
            for e in self.g[v]:
                if e.cap > 0 and self.level[e.to] == -1:
                    self.level[e.to] = self.level[v] + 1
                    q.append(e.to)

        return self.level[t] != -1

    def dfs(self, v, t, f):
        if v == t:
            return f

        while self.it[v] < len(self.g[v]):
            e = self.g[v][self.it[v]]

            if e.cap > 0 and self.level[e.to] == self.level[v] + 1:
                d = self.dfs(e.to, t, min(f, e.cap))
                if d:
                    e.cap -= d
                    self.g[e.to][e.rev].cap += d
                    return d

            self.it[v] += 1

        return 0

    def max_flow(self, s, t):
        flow = 0
        while self.bfs(s, t):
            self.it = [0] * self.n

            while True:
                f = self.dfs(s, t, INF)
                if f == 0:
                    break
                flow += f

        return flow


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


N, M, X = MI()
d = Dinic(N)

for i in range(M):
    S, E, C = MI()
    S -= 1
    E -= 1
    d.add_edge(S, E, C)
f = d.max_flow(0, N - 1)
if f == 0:
    print('Orz Ni Jinan Saint Cow!')
else:
    cnt = math.ceil(X / f)
    print(f, cnt)
