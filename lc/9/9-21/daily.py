# https://leetcode.cn/problems/find-x-value-of-array-i


class Solution:
    def resultArray(self, nums: list[int], k: int) -> list[int]:
        n = len(nums)

        dp = [0 for _ in range(k)]
        if k == 1:
            return [n * (n + 1) // 2]

        ans = [0 for _ in range(k)]
        for i in range(n):
            ndp = [0 for _ in range(k)]
            for j in range(k):
                ndp[(j * nums[i]) % k] += dp[j]
            ndp[nums[i] % k] += 1
            dp = ndp
            for j in range(k):
                ans[j] += dp[j]
            # print(dp)
        return ans
