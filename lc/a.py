class Solution:
    def minOperations(self, nums: list[int], sum: int) -> int:
        n = len(nums)
        Inf = 10**18
        dp = [[Inf for i in range(sum + 1)] for j in range(n + 1)]

        dp[0][0] = 0

        for i in range(1, n + 1):

            for j in range(sum + 1):
                x = nums[i - 1]
                # print(f"x = {x}")
                dp[i][j] = dp[i - 1][j]
                for k in range(10):
                    if x == 0:
                        break
                    if j >= x:
                        dp[i][j] = min(dp[i][j], dp[i - 1][j - x] + k)
                    x >>= 1

                x = nums[i - 1]
                for k in range(10):
                    if x > sum:
                        break
                    if j >= x:
                        dp[i][j] = min(dp[i][j], dp[i - 1][j - x] + k)
                    x <<= 1
        return -1 if dp[n][sum] == Inf else dp[n][sum]


s = Solution()
print(s.minOperations([10, 2], 13))
print(s.minOperations([6, 3], 8))
print(s.minOperations([2, 2], 7))
