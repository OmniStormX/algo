def manacher(s):
    p = ["#"]
    for c in s:
        p.append(c)
        p.append("#")
    n = len(p)
    d = [1] * n
    r = 0
    l = 0
    i = 0
    while i < n:
        r = min(r - i, d[l + r - i])
        while i + d[i] < n and i - d[i] >= 0 and p[i + d[i]] == p[i - d[i]]:
            d[i] += 1
        l = i - d[i] + 1
        r = i + d[i] - 1
        i += 1
    return d


class Solution:
    def maxPalindromes(self, s: str, k: int) -> int:
        r = manacher(s)
        n = len(r)
        dp = [0] * (n + 1)
        R = 0
        k_odd = 0
        k_even = 0
        if k & 1:
            k_odd = 2 * k + 1
            k_even = (k + 1) * 2 + 1
        else:
            k_odd = 2 * (k + 1) + 1
            k_even = 2 * k + 1
        # @a@a@a@
        # a@a@a@a
        for i, x in enumerate(r):
            rr = 0
            if i & 1 == 1:
                if x * 2 - 1 < k_odd:
                    continue
                rr = (k_odd + 1) // 2
                x = rr
            else:
                if x * 2 - 1 < k_even:
                    continue
                rr = (k_even + 1) // 2
            x = rr
            while R < i + x - 1:
                R += 1
                dp[R] = max(dp[R], dp[R - 1])
            dp[i + x - 1] = max(dp[i + x - 2], dp[i - x + 1] + 1)
        while R < n - 1:
            R += 1
            dp[R] = max(dp[R], dp[R - 1])
        return dp[n - 1]
