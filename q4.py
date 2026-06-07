from typing import List


class Solution:
    def maxScore(self, nums: List[int], maxVal: int) -> int:
        if len(nums) == 1:
            return nums[0]
        ans = 0
        ans = max(nums) - len(nums) + 1
        print("ans = ", ans)
        d = [0 for i in range(100001)]

        MAX = max(nums)
        for x in nums:
            d[x] += 1

        M = min(100000, maxVal)
        # print(d[:M + 10])
        # print("M = ", M)
        for i in range(2, M + 1):
            q = 0
            mx = 0
            for j in range(i, 100001, i):
                q += d[j]
                if j + i > M and j <= M:
                    mx = j
            print(mx, q)
            if mx < MAX:
                mx = MAX
            # print("ans:", mx - q)
            print(mx, q)
            ans = max(ans, mx - q - 1)
            if d[mx] > 0:
                ans = max(ans, mx - q)
        return ans


s = Solution()

print(s.maxScore([3, 4, 6], 5))
