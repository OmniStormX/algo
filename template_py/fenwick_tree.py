class Fenwick:

    def __init__(self, n):
        self.t = [0 for i in range(n + 1)]
        self.N = n

    def add(self, x, v):
        while x <= self.N:
            self.t[x] += v
            x += x & -x

    def query(self, l, r):
        return self._query(r) - self._query(l - 1)

    def _query(self, x):
        assert x >= 0
        ans = 0
        while x > 0:
            ans += self.t[x]
            x -= x & -x
        return ans
