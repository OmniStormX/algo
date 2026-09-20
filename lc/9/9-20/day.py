# https://leetcode.cn/problems/four-divisors

MX = 100001
M = [1 for i in range(MX)]
S = [1 for i in range(MX)]
for j in range(2, MX):
    for i in range(j, MX, j):
        M[i] += 1
        S[i] += j


class Solution:
    def sumFourDivisors(self, nums: list[int]) -> int:
        ans = 0
        for x in nums:
            if M[x] == 4:
                ans += S[x]
        return ans
