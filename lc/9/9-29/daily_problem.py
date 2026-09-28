class Solution:
    def hasValidPath(self, grid: list[list[str]]) -> bool:
        n = len(grid)
        m = len(grid[0])
        if grid[0][0] == ")":
            return False
        dp = [[0 for _ in range(m)] for __ in range(2)]
        dp[0][0] |= 2
        for i in range(n):
            for j in range(m):
                if not (i == 0 and j == 0):
                    dp[i & 1][j] = 0
                if grid[i][j] == "(":
                    if j > 0:
                        dp[i & 1][j] |= dp[i & 1][j - 1] << 1
                    if i > 0:
                        dp[i & 1][j] |= dp[(i - 1) & 1][j] << 1
                else:
                    if j > 0:
                        dp[i & 1][j] |= dp[i & 1][j - 1] >> 1
                    if i > 0:
                        dp[i & 1][j] |= dp[(i - 1) & 1][j] >> 1
        return (dp[(n - 1) & 1][m - 1] & 1) == 1
