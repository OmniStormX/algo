# https://leetcode.cn/problems/maximum-nesting-depth-of-two-valid-parentheses-strings/
class Solution:
    def maxDepthAfterSplit(self, seq: str) -> list[int]:
        n = len(seq)
        a = [0] * n
        for i in range(n):
            if seq[i] == "(":
                a[i] = i & 1
            else:
                a[i] = (i + 1) & 1
        return a
