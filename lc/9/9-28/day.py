class Solution:
    def maxDepth(self, s: str) -> int:
        ans = 0
        cur = 0
        for ch in s:
            if ch == "(":
                cur += 1
                ans = max(ans, cur)
            elif ch == ")":
                cur -= 1
        return ans
