class Solution:
    def numberOfSets(self, n: int, k: int) -> int:
        M = 10**9 + 7
        dp = [0 for _ in range(n + 1)]
        for i in range(n):
            dp[i] = 1
        # for i in range(n):
        #     dp[i][0] = i + 1
        # dp[i][j] =\sum dp[0][j - 1] ~ dp[i - 1][j - 1] + dp[i - 1][j]
        #

        for j in range(1, k + 1):
            ndp = [0 for i in range(n + 1)]
            for i in range(n):
                if i > 0:
                    ndp[i] = (dp[i - 1] + ndp[i - 1]) % M
            for i in range(n):
                ndp[i] += ndp[i - 1]
                ndp[i] %= M
            dp = ndp
        return dp[n - 1]
