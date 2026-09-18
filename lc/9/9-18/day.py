# https://leetcode.cn/problems/maximum-number-of-non-overlapping-substrings/

from bisect import bisect_left
from cmath import inf
from collections import defaultdict


class Solution:
    def maxNumOfSubstrings(self, s: str) -> list[str]:
        n = len(s)
        pos = defaultdict(list)
        for i, b in enumerate(s):
            pos[b].append(i)

        # 构建有向图
        g = defaultdict(list)
        for i, p in pos.items():
            l, r = p[0], p[-1]
            for j, q in pos.items():
                if j == i:
                    continue
                k = bisect_left(q, l)
                # [l, r] 包含第 j 个小写字母
                if k < len(q) and q[k] <= r:
                    g[i].append(j)

        # 遍历有向图
        def dfs(x: str) -> None:
            nonlocal l, r
            vis.add(x)
            p = pos[x]
            l = min(l, p[0])  # 合并区间
            r = max(r, p[-1])
            for y in g[x]:
                if y not in vis:
                    dfs(y)

        interval = {}
        for i, p in pos.items():
            # 如果要包含第 i 个小写字母，最终得到的区间是什么？
            vis = set()
            l, r = inf, 0
            dfs(i)
            interval[r] = l

        dp = [0] * (n + 1)
        ans = [0] * (n + 1)

        for i in range(n):
            dp[i] = dp[i - 1]
            ans[i] = ans[i - 1]
            if i not in interval:
                continue
            L = interval[i]
            if L == -1:
                continue

            prev_dp = dp[L - 1] if L > 0 else 0
            prev_ans = ans[L - 1] if L > 0 else 0

            if dp[i] < prev_dp + 1:
                dp[i] = prev_dp + 1
                ans[i] = prev_ans + (i - L + 1)
            elif dp[i] == prev_dp + 1:
                new_ans = prev_ans + (i - L + 1)
                ans[i] = min(ans[i], new_ans)

        cur = n - 1
        res = []
        while cur >= 0:
            while cur > 0 and dp[cur - 1] == dp[cur] and ans[cur - 1] == ans[cur]:
                cur -= 1
            if cur not in interval:
                break
            L = interval[cur]
            if L == -1:
                break
            res.append(s[L : cur + 1])
            cur = L - 1

        return res
