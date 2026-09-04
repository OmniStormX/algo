import sys

def I():
    return int(sys.stdin.readline())

def LI():
    return list(map(int, sys.stdin.readline().split()))


class Solution:
    def stoneGameVIII(self, stones: list[int]) -> int:
        n = len(stones)

        # 原地前缀和
        for i in range(1, n):
            stones[i] += stones[i - 1]

        # dp[n - 1]
        dp = stones[-1]

        # dp[i] 只依赖 dp[i + 1]
        for i in range(n - 2, 0, -1):
            dp = max(dp, stones[i] - dp)

        return dp


n = I()
a = LI()

print(Solution().stoneGameVIII(a))