class Solution:
    def hasValidPath(self, grid: list[list[str]]) -> bool:
        n = len(grid)
        m = len(grid[0])

        if grid[0][0] == ")":
            return False

        dp = [[0 for _ in range(m)] for __ in range(n)]

        dp[0][0] |= 2

        for i in range(n):
            for j in range(m):
                if j > 0:
                    if grid[i][j] == "(":
                        dp[i][j] |= dp[i][j - 1] << 1
                    else:
                        dp[i][j] |= dp[i][j - 1] >> 1
                if i > 0:
                    if grid[i][j] == "(":
                        dp[i][j] |= dp[i - 1][j] << 1
                    else:
                        dp[i][j] |= dp[i - 1][j] >> 1
        return (dp[n - 1][m - 1] & 1) == 1
