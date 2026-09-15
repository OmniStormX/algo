M = 10**9 + 7


def qpow(x, y):
    ans = 1
    while y:
        if y & 1:
            ans = ans * x % M
        y >>= 1
        x = x * x % M
    return ans


class Solution:
    def numberOfSets(self, n: int, k: int) -> int:
        d = 1
        q = 1
        for i in range(n + k - 1, n - k - 1, -1):
            d = d * i % M

        for i in range(1, 2 * k + 1):
            q = q * i % M
        return d * qpow(q, M - 2) % M
