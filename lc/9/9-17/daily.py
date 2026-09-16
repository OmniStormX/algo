# https://leetcode.cn/problems/find-two-non-overlapping-sub-arrays-each-with-target-sum
from collections import defaultdict


class Solution:
    def minSumOfLengths(self, arr: list[int], target: int) -> int:
        d = defaultdict(int)
        n = len(arr)
        pre = 0
        suf = 0
        l = [10**9] * (n + 1)
        r = [10**9] * (n + 1)
        d[0] = -1
        for i in range(n):
            pre += arr[i]
            l[i] = l[i - 1]
            if pre - target in d:
                l[i] = min(l[i], i - d[pre - target])
            d[pre] = i
        dd = defaultdict(int)
        dd[0] = n
        ans = 10**9
        for i in range(n - 1, -1, -1):
            suf += arr[i]
            r[i] = r[i + 1]
            if suf - target in dd:
                r[i] = min(r[i], dd[suf - target] - i)
            dd[suf] = i
            if i - 1 >= 0:
                ans = min(ans, l[i - 1] + r[i])
        if ans >= 10**9:
            ans = -1
        return ans
